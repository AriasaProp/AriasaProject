// 
if (!localStorage.getItem('theme-colors'))
  localStorage.setItem('theme-colors','#b3b3b3#efefef#eee#000#121212#262626#ccc#f2f2f2');
if (!localStorage.getItem('theme-fontSize'))
  localStorage.setItem('theme-fontSize', '19');
function updateColors() {
  var clr = localStorage.getItem('theme-colors');
  var clrs = clr.split('#').filter(j => { return j !== ''; });
  if (clrs.length === 8) {
    for (let j = 0; j < clrs.length; ++j) clrs[j] = '#' + clrs[j];
    document.documentElement.style.setProperty('--navColor', clrs[0]);
    document.documentElement.style.setProperty('--bodyColor', clrs[1]);
    document.documentElement.style.setProperty('--accentColor', clrs[2]);
    document.documentElement.style.setProperty('--fontColor', clrs[3]);
    document.documentElement.style.setProperty('--navColorDark', clrs[4]);
    document.documentElement.style.setProperty('--bodyColorDark', clrs[5]);
    document.documentElement.style.setProperty('--accentColorDark', clrs[6]);
    document.documentElement.style.setProperty('--fontColorDark', clrs[7]);
  }
}
function updateFontSize() {
  var fz = localStorage.getItem('theme-fontSize');
  document.documentElement.style.setProperty('--F1', fz + 'pt');
}
updateColors();
updateFontSize();
