var theme_val = {
  primaryColor: 210,
  accentColor: 180,
  fontColor: 240,
  fontSize: 14
};
const CLR_DEG = 30;

// default data
let localTheme = localStorage.getItem('Theme');
if (localTheme) {
  theme_val = JSON.parse(localTheme);
} else {
  localStorage.setItem('Theme', JSON.stringify(theme_val));
}
Object.keys(theme_val).forEach((k) => {
  document.documentElement.style.setProperty(`--${k}`, theme_val[k]);
});
