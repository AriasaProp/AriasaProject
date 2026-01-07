Te.innerHTML += 'started \n';
WebAssembly.instantiateStreaming(fetch("third/core_basic_windows.wasm"), {
  //env: make_environment({})
}).then(wasm => {
  Te.innerHTML += JSON.stringify(wasm);
}).catch(err => {
  Te.innerHTML += err;
});
/*
function make_environment(env) {
  return new Proxy(env, {
    get(target, prop, receiver) {
      if (env[prop] !== undefined) {
        return env[prop].bind(env);
      }
      return (...args) => throw new Error(`NOT IMPLEMENTED: ${prop} ${args}`);
    }
  });
}
*/  