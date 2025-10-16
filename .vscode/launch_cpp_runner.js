const { spawn } = require('child_process');
const path = require('path');

const workspaceFolder = process.env.PWD || process.cwd();
const executable = path.join(workspaceFolder, 'build', 'app', 'pong', 'druid-pong');

console.log(`Running: ${executable}`);

const child = spawn(executable, [], {
    cwd: workspaceFolder,
    stdio: 'inherit'
});

child.on('close', (code) => {
    console.log(`druid-pong exited with code ${code}`);
    process.exit(code);
});

child.on('error', (err) => {
    console.error(`Failed to start druid-pong: ${err.message}`);
    process.exit(1);
});