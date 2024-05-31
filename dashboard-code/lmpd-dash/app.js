const { app, BrowserWindow } = require('electron');

let appWindow;

function createWindow() {
    appWindow = new BrowserWindow({
        width: 1300,
        height: 700,
        autoHideMenuBar: true // This option hides the menu bar
    });

    appWindow.loadFile('dist/lmpd-dash/index.html');

    // This line ensures the menu bar is hidden by default
    appWindow.setMenuBarVisibility(false);

    appWindow.on('closed', function() {
        appWindow = null;
    });
}

app.whenReady().then(() => {
    createWindow();
});
