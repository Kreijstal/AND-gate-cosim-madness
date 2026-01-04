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
// \src: counter.sv:1.8
struct p_counter : public module {
	// \src: counter.sv:5.24
	// \hdlname: count
	/*output*/ wire<8> p_count;
	// \src: counter.sv:4.24
	// \hdlname: en
	/*input*/ value<1> p_en;
	// \src: counter.sv:3.24
	// \hdlname: rst
	/*input*/ value<1> p_rst;
	// \src: counter.sv:2.24
	// \hdlname: clk
	/*input*/ value<1> p_clk;
	value<1> prev_p_clk;
	bool posedge_p_clk() const {
		return !prev_p_clk.slice<0>().val() && p_clk.slice<0>().val();
	}
	p_counter(interior) {}
	p_counter() {
		reset();
	};

	void reset() override;

	bool eval(performer *performer = nullptr) override;

	template<class ObserverT>
	bool commit(ObserverT &observer) {
		bool changed = false;
		if (p_count.commit(observer)) changed = true;
		prev_p_clk = p_clk;
		return changed;
	}

	bool commit() override {
		observer observer;
		return commit<>(observer);
	}

	void debug_eval();

	void debug_info(debug_items *items, debug_scopes *scopes, std::string path, metadata_map &&cell_attrs = {}) override;
}; // struct p_counter

void p_counter::reset() {
}

bool p_counter::eval(performer *performer) {
	bool converged = true;
	bool posedge_p_clk = this->posedge_p_clk();
	// cells $driver$count $procmux$8 $procmux$4 $procmux$1 $1
	if (posedge_p_clk) {
		p_count.next = (p_rst ? value<8>{0u} : (p_rst ? value<8>{0u} : (p_en ? add_uu<8>(p_count.curr, value<8>{0x1u}) : p_count.curr)));
	}
	return converged;
}

void p_counter::debug_eval() {
}

CXXRTL_EXTREMELY_COLD
void p_counter::debug_info(debug_items *items, debug_scopes *scopes, std::string path, metadata_map &&cell_attrs) {
	assert(path.empty() || path[path.size() - 1] == ' ');
	if (scopes) {
		scopes->add(path.empty() ? path : path.substr(0, path.size() - 1), "counter", metadata_map({
			{ "top", UINT64_C(1) },
			{ "src", "counter.sv:1.8" },
		}), std::move(cell_attrs));
	}
	if (items) {
		items->add(path, "count", "src\000scounter.sv:5.24\000", p_count, 0, debug_item::OUTPUT|debug_item::DRIVEN_SYNC);
		items->add(path, "en", "src\000scounter.sv:4.24\000", p_en, 0, debug_item::INPUT|debug_item::UNDRIVEN);
		items->add(path, "rst", "src\000scounter.sv:3.24\000", p_rst, 0, debug_item::INPUT|debug_item::UNDRIVEN);
		items->add(path, "clk", "src\000scounter.sv:2.24\000", p_clk, 0, debug_item::INPUT|debug_item::UNDRIVEN);
	}
}

} // namespace cxxrtl_design

extern "C"
cxxrtl_toplevel cxxrtl_design_create() {
	return new _cxxrtl_toplevel { std::unique_ptr<cxxrtl_design::p_counter>(new cxxrtl_design::p_counter) };
}
