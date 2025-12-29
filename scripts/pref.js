const cp = Math.PI / 180;
function clrPickC(p, rad) {
  const r = Math.min(p.s.x, p.s.y) * 0.9;
  const d = r * 0.7; // thickness of pallete
  const ctx = p.ctx;
  for (i = 0; i < 360; i++) {
    ctx.fillStyle = `hsl(${i}, 100%, 50%)`;
    ctx.beginPath();
    let inArcX  = Math.cos(cp * (i - 0.5));
    let inArcY  = Math.sin(cp * (i - 0.5));
    ctx.moveTo(p.s.x + d * inArcX,p.s.y + d * inArcY);
    ctx.lineTo(p.s.x + r * inArcX,p.s.y + r * inArcY);
    inArcX  = Math.cos(cp * (i + 0.5));
    inArcY  = Math.sin(cp * (i + 0.5));
    ctx.lineTo(p.s.x + r * inArcX,p.s.y + r * inArcY);
    ctx.lineTo(p.s.x + d * inArcX,p.s.y + d * inArcY);
    ctx.closePath();
    ctx.fill();
  }
  ctx.fillStyle = `hsl(${rad}, 100%, 50%)`;
  ctx.beginPath();
  ctx.arc(p.s.x, p.s.y, d, 0, Math.PI * 2);
  ctx.closePath();
  ctx.fill();
  ctx.strokeStyle = 'black';
  ctx.beginPath();
  ctx.moveTo(p.s.x + (d - 5) * Math.cos(cp * (rad + 0.0)), p.s.y + (d - 5) * Math.sin(cp * (rad + 0.0)));
  ctx.lineTo(p.s.x + (r + 5) * Math.cos(cp * (rad + 2.5)), p.s.y + (r + 5) * Math.sin(cp * (rad + 2.5)));
  ctx.lineTo(p.s.x + (r + 5) * Math.cos(cp * (rad - 2.5)), p.s.y + (r + 5) * Math.sin(cp * (rad - 2.5)));
  ctx.closePath();
  ctx.fill();
  ctx.stroke();
  ctx.fillStyle = 'black';
  ctx.textAlign = 'center';
  ctx.font = "normal 700 1.1em Roboto, sans-serif";
  ctx.textBaseline = 'bottom';
  ctx.fillText(p.n, p.s.x, p.s.y);
  ctx.textBaseline = 'top';
  ctx.fillText('Color', p.s.x, p.s.y);
}
function redrawClrPickC(p, rad) {
  ctx.clearRect(0, 0, p.canvas.clientWidth, p.canvas.clientHeight);
  clrPickC(p, parseInt(rad));
}
let colorPickerConstant = [
  {k: 'primaryColor',n: 'Primary'},
  {k: 'accentColor',n: 'Accent'},
  {k: 'fontColor',n: 'Font'}];
colorPickerConstant.forEach(p => {
  p.obj = sett.elements[p.k];
  p.canvas = p.obj.parentNode;
  p.ctx = p.canvas.getContext('2d');
  p.s = {
    x: p.canvas.clientWidth * 0.5,
    y: p.canvas.clientHeight * 0.5
  };
  clrPickC(p, parseInt(theme_val[p.k]));
  p.obj.value = parseInt(theme_val[p.k]);
  p.canvas.addEventListener('click', (e) => {
    let rad = Math.round(180 + Math.atan2(p.s.y - e.offsetY, p.s.x - e.offsetX) / cp);
    if (parseInt(p.obj.value) === rad) return;
    redrawClrPickC(p, parseInt(rad));
    p.obj.value = rad;
    sett.dispatchEvent(new Event('change'));
  });
});
let areset = sett.querySelector('button[type="reset"]');
let asubmit = sett.querySelector('button[type="submit"]');
sett.addEventListener('change', (e) => {
  areset.disabled = false;
  asubmit.disabled = false;
});
sett.addEventListener('reset', (e) => {
  areset.disabled = true;
  asubmit.disabled = true;
  
  colorPickerConstant.forEach(p => {
    redrawClrPickC(p, theme_val[p.k]);
    p.obj.value = parseInt(theme_val[p.k]);
  });
});
sett.addEventListener('submit', (e) => {
  areset.disabled = true;
  asubmit.disabled = true;
  e.preventDefault();
  Object.keys(theme_val).forEach((k) => {
    let a = sett.elements[k];
    if (theme_val[k] !== a.value) {
      document.documentElement.style.setProperty(`--${k}`, a.value);
      theme_val[k] = a.defaultValue = a.value;
    }
  });
  localStorage.setItem('Theme', JSON.stringify(theme_val));
  colorPickerConstant.forEach(p => {
    redrawClrPickC(p, theme_val[p.k]);
    p.obj.value = parseInt(theme_val[p.k]);
  });
});
