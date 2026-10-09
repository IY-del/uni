const num = Math.floor(Math.random() * 6 + 1);

const levels = {
    1: '大吉',
    2: '中吉',
    3: '小吉',
    4: '末吉',
    5: '凶',
    6: '大凶'
};

console.log('あなたの運勢は' + levels[num] + 'です');
