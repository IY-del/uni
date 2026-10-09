const LEVELS = ['大吉', '中吉', '小吉', '末吉', '凶', '大凶'];

function getOmikuji() {
    const index = Math.floor(Math.random() * LEVELS.length);
    return LEVELS[index];
}

module.exports = getOmikuji;
