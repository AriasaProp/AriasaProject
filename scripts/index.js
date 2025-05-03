
// for index 

fetch('texts/dummy.txt')
  .then(response => response.text())
  .then(text => {
    document.getElementById("result-container").innerHTML = text;
    
  })

function searchSubmit(e) {
  e.preventDefault();
  const formElement = event.target;
  const formData = new FormData(formElement);
  const s = formData.get('s');

  console.log(s);
  document.getElementById("result-container").innerHTML = s;
  
  fetch('https://jsonplaceholder.typicode.com/posts/1')
      .then(response => response.json())
      .then(json => console.log(json))
}
/*
const nav = document.getElementById('nav-main')
var intersect = new IntersectionObserver(entries => {
	entries.forEach(entry => {
		nav.classList.toggle('nav-present', entry.isIntersecting)
	})
}, {
	threshold: .5,
}).observe(document.getElementById('header'))
*/