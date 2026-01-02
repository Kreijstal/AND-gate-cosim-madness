from amaranth.hdl import Signal
from amaranth.sim import Simulator
from amaranth.sim.pysim import PySimEngine


class ExternalBackend:
    def eval(self, inputs):
        raise NotImplementedError


class ExternalEngine(PySimEngine):
    def __init__(self, design, backend, input_map, output_map):
        super().__init__(design)
        self._backend = backend
        self._input_map = input_map
        self._output_map = output_map
        self._max_external_settle = 3

    def _capture_inputs(self):
        inputs = {}
        for name, signal in self._input_map.items():
            inputs[name] = int(self.get_value(signal))
        return inputs

    def _apply_outputs(self, outputs):
        changed = False
        for name, signal in self._output_map.items():
            if name not in outputs:
                continue
            value = int(outputs[name])
            if int(self.get_value(signal)) != value:
                self.set_value(signal, value)
                changed = True
        return changed

    def step_design(self):
        super().step_design()
        if self._backend is None:
            return
        for _ in range(self._max_external_settle):
            outputs = self._backend.eval(self._capture_inputs())
            if outputs is None:
                break
            if not self._apply_outputs(outputs):
                break
            super().step_design()


class ExternalSimulator(Simulator):
    def __init__(self, toplevel, backend, *, inputs, outputs):
        super().__init__(toplevel, engine="pysim")
        input_map = self._normalize_mapping(inputs)
        output_map = self._normalize_mapping(outputs)
        self._engine = ExternalEngine(self._design, backend, input_map, output_map)

    @staticmethod
    def _normalize_mapping(mapping):
        norm = {}
        for name, signal in mapping.items():
            if not isinstance(signal, Signal):
                raise TypeError(f"{name} is not a Signal")
            norm[name] = signal
        return norm
