class PageApp {
  constructor() {
    this.init();
  }

  init() {
    console.log("Page loaded:", window.location.pathname);
  }
}

document.addEventListener("DOMContentLoaded", () => {
  new PageApp();
});
