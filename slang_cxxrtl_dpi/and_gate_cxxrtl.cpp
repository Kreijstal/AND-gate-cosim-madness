#include <cxxrtl/cxxrtl.h>

#if defined(CXXRTL_INCLUDE_CAPI_IMPL) || \
    defined(CXXRTL_INCLUDE_VCD_CAPI_IMPL)
#include <cxxrtl/capi/cxxrtl_capi.cc>
#endif

#if defined(CXXRTL_INCLUDE_VCD_CAPI_IMPL)
#include <cxxrtl/capi/cxxrtl_capi_vcd.cc>
#endif

using namespace cxxrtl_yosys;

namespace cxxrtl_design {

// \top: 1
// \src: and_gate.sv:1.8
struct p_and__gate : public module {
	// \src: and_gate.sv:4.18
	// \hdlname: y
	/*output*/ value<1> p_y;
	// \src: and_gate.sv:3.18
	// \hdlname: b
	/*input*/ value<1> p_b;
	// \src: and_gate.sv:2.18
	// \hdlname: a
	/*input*/ value<1> p_a;
	p_and__gate(interior) {}
	p_and__gate() {
		reset();
	};

	void reset() override;

	bool eval(performer *performer = nullptr) override;

	template<class ObserverT>
	bool commit(ObserverT &observer) {
		bool changed = false;
		return changed;
	}

	bool commit() override {
		observer observer;
		return commit<>(observer);
	}

	void debug_eval();

	void debug_info(debug_items *items, debug_scopes *scopes, std::string path, metadata_map &&cell_attrs = {}) override;
}; // struct p_and__gate

void p_and__gate::reset() {
}

bool p_and__gate::eval(performer *performer) {
	bool converged = true;
	// \src: and_gate.sv:8.13-8.18
	// cell $1
	p_y = and_uu<1>(p_a, p_b);
	return converged;
}

void p_and__gate::debug_eval() {
}

CXXRTL_EXTREMELY_COLD
void p_and__gate::debug_info(debug_items *items, debug_scopes *scopes, std::string path, metadata_map &&cell_attrs) {
	assert(path.empty() || path[path.size() - 1] == ' ');
	if (scopes) {
		scopes->add(path.empty() ? path : path.substr(0, path.size() - 1), "and_gate", metadata_map({
			{ "top", UINT64_C(1) },
			{ "src", "and_gate.sv:1.8" },
		}), std::move(cell_attrs));
	}
	if (items) {
		items->add(path, "y", "src\000sand_gate.sv:4.18\000", p_y, 0, debug_item::OUTPUT|debug_item::DRIVEN_COMB);
		items->add(path, "b", "src\000sand_gate.sv:3.18\000", p_b, 0, debug_item::INPUT|debug_item::UNDRIVEN);
		items->add(path, "a", "src\000sand_gate.sv:2.18\000", p_a, 0, debug_item::INPUT|debug_item::UNDRIVEN);
	}
}

} // namespace cxxrtl_design

extern "C"
cxxrtl_toplevel cxxrtl_design_create() {
	return new _cxxrtl_toplevel { std::unique_ptr<cxxrtl_design::p_and__gate>(new cxxrtl_design::p_and__gate) };
}
