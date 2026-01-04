#!/usr/bin/env python3
"""
Generate CXXRTL DPI wrapper files from a SystemVerilog module.

Usage: python generate_cxxrtl_dpi.py <module.sv> [--top <module_name>]

Generates:
  - <module>_proxy.sv  : DPI-C proxy module with same interface
  - <module>_dpi.cpp   : C++ DPI wrapper calling CXXRTL
"""

import argparse
import json
import subprocess
import sys
import tempfile
from pathlib import Path


def run_yosys_json(sv_file: str, top_module: str | None = None) -> dict:
    """Use Yosys with slang to parse SV and output JSON."""
    with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as f:
        json_file = f.name

    top_cmd = f"-r {top_module}" if top_module else ""
    cmd = f'yosys -m slang -Q {top_cmd} -p "read_slang {sv_file}; write_json {json_file}" 2>&1'
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Yosys error:\n{result.stdout}\n{result.stderr}", file=sys.stderr)
        sys.exit(1)

    with open(json_file) as f:
        data = json.load(f)
    Path(json_file).unlink()
    return data


def extract_module_info(yosys_json: dict, top_module: str | None = None) -> dict:
    """Extract module interface from Yosys JSON output."""
    modules = yosys_json.get("modules", {})

    if not modules:
        raise ValueError("No modules found in design")

    # Find the top module
    if top_module:
        if top_module not in modules:
            raise ValueError(f"Module '{top_module}' not found. Available: {list(modules.keys())}")
        mod_name = top_module
    else:
        # Use the first (or only) module, or one marked as top
        for name, mod in modules.items():
            if mod.get("attributes", {}).get("top"):
                mod_name = name
                break
        else:
            mod_name = list(modules.keys())[0]

    mod = modules[mod_name]
    ports = []

    for port_name, port_info in mod.get("ports", {}).items():
        direction = port_info.get("direction", "input")
        bits = port_info.get("bits", [])
        width = len(bits)

        ports.append({
            "name": port_name,
            "direction": direction,
            "width": width,
            "is_clock": port_name.lower() in ("clk", "clock", "clk_i", "i_clk"),
            "is_reset": port_name.lower() in ("rst", "reset", "rst_i", "i_rst", "rst_n", "reset_n"),
        })

    return {
        "name": mod_name,
        "ports": ports,
        "has_clock": any(p["is_clock"] for p in ports),
    }


def generate_proxy_sv(mod_info: dict) -> str:
    """Generate the SystemVerilog proxy module with DPI-C."""
    name = mod_info["name"]
    ports = mod_info["ports"]

    # Build port list
    port_decls = []
    for p in ports:
        dir_kw = "input" if p["direction"] == "input" else "output"
        if p["width"] == 1:
            port_decls.append(f"    {dir_kw} wire {p['name']}")
        else:
            port_decls.append(f"    {dir_kw} wire [{p['width']-1}:0] {p['name']}")

    # Build DPI function signature
    # For outputs, we use return value for single output, or output params for multiple
    inputs = [p for p in ports if p["direction"] == "input"]
    outputs = [p for p in ports if p["direction"] == "output"]

    # DPI function parameters
    dpi_params = []
    for p in inputs:
        if p["width"] <= 32:
            dpi_params.append(f"input int {p['name']}_i")
        else:
            dpi_params.append(f"input longint {p['name']}_i")

    for p in outputs:
        if p["width"] <= 32:
            dpi_params.append(f"output int {p['name']}_o")
        else:
            dpi_params.append(f"output longint {p['name']}_o")

    dpi_func_name = f"cxxrtl_{name}"

    # Build wire extensions and assignments
    wire_decls = []
    call_args = []
    output_assigns = []

    for p in inputs:
        ext_width = 64 if p["width"] > 32 else 32
        wire_decls.append(f"    wire [{ext_width-1}:0] {p['name']}_ext = {{{{{ext_width-p['width']}{{1'b0}}}}, {p['name']}}};")
        call_args.append(f"{p['name']}_ext")

    for p in outputs:
        ext_width = 64 if p["width"] > 32 else 32
        # Use logic (not wire) for DPI output parameters
        wire_decls.append(f"    logic [{ext_width-1}:0] {p['name']}_ext;")
        call_args.append(f"{p['name']}_ext")
        if p["width"] == 1:
            output_assigns.append(f"    assign {p['name']} = {p['name']}_ext[0];")
        else:
            output_assigns.append(f"    assign {p['name']} = {p['name']}_ext[{p['width']-1}:0];")

    # Generate the module
    sv = f"""`timescale 1ns / 1ps

// Auto-generated CXXRTL DPI proxy for module '{name}'
module {name}(
{',\n'.join(port_decls)}
);

    import "DPI-C" function void {dpi_func_name}({', '.join(dpi_params)});

{chr(10).join(wire_decls)}

    always_comb begin
        {dpi_func_name}({', '.join(call_args)});
    end

{chr(10).join(output_assigns)}

endmodule
"""
    return sv


def generate_dpi_cpp(mod_info: dict, cxxrtl_header: str) -> str:
    """Generate the C++ DPI wrapper."""
    name = mod_info["name"]
    ports = mod_info["ports"]
    has_clock = mod_info["has_clock"]

    # CXXRTL uses p_ prefix and escapes special chars
    def cxxrtl_port_name(port_name: str) -> str:
        return f"p_{port_name.replace('_', '__')}"

    inputs = [p for p in ports if p["direction"] == "input"]
    outputs = [p for p in ports if p["direction"] == "output"]

    # DPI function parameters
    dpi_params = []
    for p in inputs:
        c_type = "int64_t" if p["width"] > 32 else "int"
        dpi_params.append(f"{c_type} {p['name']}_i")
    for p in outputs:
        c_type = "int64_t" if p["width"] > 32 else "int"
        dpi_params.append(f"{c_type}* {p['name']}_o")

    # Input assignments
    input_sets = []
    for p in inputs:
        pname = cxxrtl_port_name(p['name'])
        if p["width"] == 1:
            input_sets.append(f"    cxxrtl_dut.{pname}.set<bool>({p['name']}_i != 0);")
        elif p["width"] <= 32:
            input_sets.append(f"    cxxrtl_dut.{pname}.set<uint32_t>({p['name']}_i);")
        else:
            input_sets.append(f"    cxxrtl_dut.{pname}.set<uint64_t>({p['name']}_i);")

    # Output reads
    output_gets = []
    for p in outputs:
        pname = cxxrtl_port_name(p['name'])
        if p["width"] == 1:
            output_gets.append(f"    *{p['name']}_o = cxxrtl_dut.{pname}.get<bool>() ? 1 : 0;")
        elif p["width"] <= 32:
            output_gets.append(f"    *{p['name']}_o = cxxrtl_dut.{pname}.get<uint32_t>();")
        else:
            output_gets.append(f"    *{p['name']}_o = cxxrtl_dut.{pname}.get<uint64_t>();")

    # Clock handling - always step() to let CXXRTL's internal edge detection work
    # CXXRTL maintains prev_clk internally and only updates registers on edges
    clock_step = "\n    // Step simulation - CXXRTL handles edge detection internally\n    cxxrtl_dut.step();\n"

    cxxrtl_class = f"p_{name.replace('_', '__')}"
    dpi_func_name = f"cxxrtl_{name}"

    cpp = f"""#include "svdpi.h"
#include <cstdint>
#include "{cxxrtl_header}"

// Static CXXRTL instance
static cxxrtl_design::{cxxrtl_class} cxxrtl_dut;

extern "C" void {dpi_func_name}({', '.join(dpi_params)}) {{
    // Set inputs
{chr(10).join(input_sets)}
{clock_step}
    // Read outputs
{chr(10).join(output_gets)}
}}
"""
    return cpp


def main():
    parser = argparse.ArgumentParser(description="Generate CXXRTL DPI wrapper files")
    parser.add_argument("sv_file", help="SystemVerilog source file")
    parser.add_argument("--top", "-t", help="Top module name (auto-detected if not specified)")
    parser.add_argument("--output-dir", "-o", default=".", help="Output directory")
    parser.add_argument("--cxxrtl-header", help="CXXRTL header filename (default: <module>_cxxrtl.cpp)")
    args = parser.parse_args()

    sv_path = Path(args.sv_file)
    if not sv_path.exists():
        print(f"Error: {sv_path} not found", file=sys.stderr)
        sys.exit(1)

    print(f"Parsing {sv_path}...")
    yosys_json = run_yosys_json(str(sv_path), args.top)
    mod_info = extract_module_info(yosys_json, args.top)

    print(f"Found module: {mod_info['name']}")
    print(f"  Ports: {', '.join(p['name'] for p in mod_info['ports'])}")
    print(f"  Has clock: {mod_info['has_clock']}")

    out_dir = Path(args.output_dir)
    mod_name = mod_info["name"]
    cxxrtl_header = args.cxxrtl_header or f"{mod_name}_cxxrtl.cpp"

    # Generate proxy.sv
    proxy_sv = generate_proxy_sv(mod_info)
    proxy_path = out_dir / f"{mod_name}_proxy.sv"
    proxy_path.write_text(proxy_sv)
    print(f"Generated: {proxy_path}")

    # Generate _dpi.cpp
    dpi_cpp = generate_dpi_cpp(mod_info, cxxrtl_header)
    dpi_path = out_dir / f"{mod_name}_dpi.cpp"
    dpi_path.write_text(dpi_cpp)
    print(f"Generated: {dpi_path}")


if __name__ == "__main__":
    main()
