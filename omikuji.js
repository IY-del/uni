const LEVELS = ['大吉', '中吉', '小吉', '末吉', '凶', '大凶'];

const COMMENTS = {
    '大吉': 'Congratulations! You will have great luck today!',
    '中吉': 'Good luck! Things will go well for you.',
    '小吉': 'Not bad! You may have some good luck.',
    '末吉': 'Be careful! Luck may not be on your side.',
    '凶': 'Bad luck! You may face some challenges.',
    '大凶': 'Very bad luck! Be cautious and take care.'
};

function getOmikuji() {
    const index = Math.floor(Math.random() * LEVELS.length);
    return LEVELS[index] || '!ERROR!';
}

function getComment(luck) {
    return COMMENTS[luck] || '';
}

module.exports = { getOmikuji, getComment };
