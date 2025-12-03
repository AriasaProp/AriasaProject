const tfz = localStorage.getItem('theme-fontSize');
if(tfz) document.getElementById('fontSize-pref').value = tfz;
const curColors = localStorage.getItem("theme-colors");
if (curColors) {
  const radios = document.querySelectorAll('input[name="themeopt"]');
  radios.forEach(r => {
    r.defaultChecked = (r.value === curColors);
  });
}
function setColorsVar(key) {
  if(localStorage.getItem('theme-colors') !== key) {
    localStorage.setItem('theme-colors', key);
    updateColors();
  }
}
function setFontSizeVar(key) {
  if(localStorage.getItem('theme-fontSize') !== key) {
    localStorage.setItem('theme-fontSize', key);
    updateFontSize();
  }
}
