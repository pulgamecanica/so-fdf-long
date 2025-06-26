function openWindow(id) {
  const win = document.getElementById(id);
  if (win) win.style.display = 'block';
}

function closeWindow(id) {
  const win = document.getElementById(id);
  if (win) win.style.display = 'none';
}

// Drag logic
document.querySelectorAll('.window').forEach(win => {
  const bar = win.querySelector('.titlebar');
  let isDragging = false, offsetX, offsetY;

  bar.addEventListener('mousedown', e => {
    isDragging = true;
    offsetX = e.clientX - win.offsetLeft;
    offsetY = e.clientY - win.offsetTop;
    win.style.zIndex = 1000;
  });

  document.addEventListener('mousemove', e => {
    if (isDragging) {
      win.style.left = (e.clientX - offsetX) + 'px';
      win.style.top = (e.clientY - offsetY) + 'px';
    }
  });

  document.addEventListener('mouseup', () => isDragging = false);
});
