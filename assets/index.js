
let memory = new WebAssembly.Memory({initial:1});
Te.innerHTML += 'started \n';

WebAssembly.instantiateStreaming(fetch("bin/wasm32/hello_triangle.wasm"), {
  /*
  console: {
    log: (offset, length) => {
      var bytes = new Uint8Array(memory.buffer, offset, length);
      var text = new TextDecoder('utf8').decode(bytes);
      Te.innerHTML += text;
    }
  },
  js: { memory: memory }
  */
}).then(wasm => {
  Te.innerHTML += wasm.instance.exports.get_value();
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