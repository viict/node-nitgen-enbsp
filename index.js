const path = require('path');

exports = module.exports = require('node-gyp-build')(__dirname);

const setSkin = exports.setSkin;
// Override setSkin to use the DLLs we are providing on node_modules path
exports.setSkin = function(skin) {
    if (skin.indexOf('/') === - 1 && skin.indexOf('\\') === -1) {
        return setSkin(path.join(__dirname, 'skins', skin));
    }
    return setSkin(skin);
}
