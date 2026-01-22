
// form
/*
let sett = document.querySelector('main form');
let areset = sett.querySelector('button[type="reset"]');
let asubmit = sett.querySelector('button[type="submit"]');
sett.addEventListener('reset', (e) => {
  areset.disabled = true;
  asubmit.disabled = true;
  document.querySelectorAll('input[name$="Color"]').forEach((x) => 
    x.style.setProperty('--thumb', x.defaultValue)
  );
});
sett.addEventListener('submit', (e) => {
  e.preventDefault();
  areset.disabled = true;
  asubmit.disabled = true;
  Object.keys(theme_val).forEach((k) => {
    let a = sett.elements[k];
    if (theme_val[k] === a.value) return;
    document.documentElement.style.setProperty(`--${k}`, a.value);
    theme_val[k] = a.defaultValue = a.value;
  });
  localStorage.setItem('Theme', JSON.stringify(theme_val));
});
sett.addEventListener('change', (e) => {
  areset.disabled = false;
  asubmit.disabled = false;
});
*/
function clrPicki(x) {
  x.style.setProperty('--thumb', x.value);
}

