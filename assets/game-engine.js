
class Vector2 {
  constructor (x, y) {
    this.x = x;
    this.y = y;
  }
  function set(v) {
    this.x -= v.x;
    this.y -= v.y;
  }
  function sub(v) {
    let r = this;
    r.x -= v.x;
    r.y -= v.y;
    return r;
  }
  function add(v) {
    let r = this;
    r.x += v.x;
    r.y += v.y;
    return r;
  }
  function scale(v) {
    let r = this;
    r.x *= v.x;
    r.y *= v.y;
    return r;
  }
  function div(v) {
    let r = this;
    r.x /= v.x;
    r.y /= v.y;
    return r;
  }
}