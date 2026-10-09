// --- 1. Web Audio API サウンドエンジン ---
class DopaAudioEngine {
  constructor() {
    this.ctx = null;
  }

  init() {
    if (!this.ctx) {
      const AudioContext = window.AudioContext || window.webkitAudioContext;
      if (AudioContext) {
        this.ctx = new AudioContext();
      }
    }
    if (this.ctx && this.ctx.state === 'suspended') {
      this.ctx.resume();
    }
  }

  playJackpot() {
    this.init();
    if (!this.ctx) return;

    const now = this.ctx.currentTime;
    const notes = [523.25, 659.25, 783.99, 1046.50, 1318.51, 1567.98]; // C5, E5, G5, C6, E6, G6

    notes.forEach((freq, i) => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();

      osc.type = 'sawtooth';
      osc.frequency.setValueAtTime(freq, now + i * 0.06);

      gain.gain.setValueAtTime(0.15, now + i * 0.06);
      gain.gain.exponentialRampToValueAtTime(0.001, now + i * 0.06 + 0.25);

      osc.connect(gain);
      gain.connect(this.ctx.destination);

      osc.start(now + i * 0.06);
      osc.stop(now + i * 0.06 + 0.25);
    });
  }

  playCoin() {
    this.init();
    if (!this.ctx) return;

    const now = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(987.77, now); // B5
    osc.frequency.setValueAtTime(1318.51, now + 0.08); // E6

    gain.gain.setValueAtTime(0.2, now);
    gain.gain.exponentialRampToValueAtTime(0.001, now + 0.35);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(now);
    osc.stop(now + 0.35);
  }

  playCrash() {
    this.init();
    if (!this.ctx) return;

    const now = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'square';
    osc.frequency.setValueAtTime(150, now);
    osc.frequency.exponentialRampToValueAtTime(40, now + 0.2);

    gain.gain.setValueAtTime(0.25, now);
    gain.gain.exponentialRampToValueAtTime(0.001, now + 0.25);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(now);
    osc.stop(now + 0.25);
  }

  playJump() {
    this.init();
    if (!this.ctx) return;

    const now = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'triangle';
    osc.frequency.setValueAtTime(260, now);
    osc.frequency.exponentialRampToValueAtTime(600, now + 0.15);

    gain.gain.setValueAtTime(0.15, now);
    gain.gain.exponentialRampToValueAtTime(0.001, now + 0.18);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(now);
    osc.stop(now + 0.18);
  }
}

// --- 2. Subway Surfers 風パルクールCanvasエンジン（操作対応） ---
class SubwayRunnerSimulator {
  constructor(canvas, onCoin, onCrash) {
    this.canvas = canvas;
    this.ctx = canvas.getContext('2d');
    this.onCoin = onCoin;
    this.onCrash = onCrash;

    this.player = {
      lane: 1,
      targetLane: 1,
      currentX: 0,
      y: 0,
      vy: 0,
      isJumping: false,
      invincibleTimer: 0
    };
    this.lanes = [-75, 0, 75];
    this.obstacles = [];
    this.coins = [];
    this.speed = 8.5;
    this.distance = 0;
    this.frame = 0;

    this.init();
  }

  init() {
    this.bindControls();
    this.loop = this.loop.bind(this);
    requestAnimationFrame(this.loop);
  }

  moveLeft() {
    if (this.player.targetLane > 0) {
      this.player.targetLane--;
    }
  }

  moveRight() {
    if (this.player.targetLane < 2) {
      this.player.targetLane++;
    }
  }

  jump() {
    if (!this.player.isJumping) {
      this.player.isJumping = true;
      this.player.vy = -13;
      return true;
    }
    return false;
  }

  bindControls() {
    // キーボード操作
    window.addEventListener('keydown', (e) => {
      if (['ArrowLeft', 'KeyA', 'a', 'A'].includes(e.code) || ['ArrowLeft', 'KeyA', 'a', 'A'].includes(e.key)) {
        this.moveLeft();
      } else if (['ArrowRight', 'KeyD', 'd', 'D'].includes(e.code) || ['ArrowRight', 'KeyD', 'd', 'D'].includes(e.key)) {
        this.moveRight();
      } else if (['ArrowUp', 'KeyW', 'w', 'W', 'Space'].includes(e.code) || ['ArrowUp', 'KeyW', 'w', 'W', ' '].includes(e.key)) {
        this.jump();
        e.preventDefault();
      }
    });

    // 画面ボタン操作（データ属性バインド）
    document.querySelectorAll('[data-control]').forEach((btn) => {
      btn.addEventListener('click', (e) => {
        e.stopPropagation();
        const action = btn.dataset.control;
        if (action === 'left') this.moveLeft();
        if (action === 'right') this.moveRight();
        if (action === 'jump') this.jump();
      });
    });

    // Canvas上のクリック/タップ操作
    this.canvas.addEventListener('pointerdown', (e) => {
      const rect = this.canvas.getBoundingClientRect();
      const clickX = e.clientX - rect.left;
      const clickY = e.clientY - rect.top;
      const width = rect.width;
      const height = rect.height;

      if (clickY < height * 0.4) {
        this.jump();
      } else if (clickX < width * 0.4) {
        this.moveLeft();
      } else if (clickX > width * 0.6) {
        this.moveRight();
      } else {
        this.jump();
      }
    });
  }

  spawnObjects() {
    if (this.frame % 32 === 0) {
      const laneIdx = Math.floor(Math.random() * 3);
      if (Math.random() > 0.4) {
        this.obstacles.push({ lane: laneIdx, z: 500, type: 'train' });
      } else {
        this.coins.push({ lane: laneIdx, z: 500 });
      }
    }
  }

  update() {
    this.frame++;
    this.distance += this.speed;

    if (this.player.invincibleTimer > 0) {
      this.player.invincibleTimer--;
    }

    // レーン移動のスムーズ補間
    const targetX = this.lanes[this.player.targetLane];
    this.player.currentX += (targetX - this.player.currentX) * 0.28;

    // ジャンプ物理
    if (this.player.isJumping) {
      this.player.y += this.player.vy;
      this.player.vy += 0.85;
      if (this.player.y >= 0) {
        this.player.y = 0;
        this.player.isJumping = false;
      }
    }

    // 障害物＆コイン進行
    for (let i = this.obstacles.length - 1; i >= 0; i--) {
      const o = this.obstacles[i];
      o.z -= this.speed;

      // 衝突判定
      if (o.z < 80 && o.z > 20 && o.lane === this.player.targetLane && this.player.invincibleTimer <= 0) {
        if (!this.player.isJumping || this.player.y > -20) {
          this.player.invincibleTimer = 45; // 約0.75秒無敵
          if (this.onCrash) this.onCrash();
        }
      }

      if (o.z < 0) this.obstacles.splice(i, 1);
    }

    for (let i = this.coins.length - 1; i >= 0; i--) {
      const c = this.coins[i];
      c.z -= this.speed;

      // コイン取得判定
      if (c.z < 70 && c.z > 15 && c.lane === this.player.targetLane) {
        if (this.onCoin) this.onCoin();
        this.coins.splice(i, 1);
        continue;
      }

      if (c.z < 0) this.coins.splice(i, 1);
    }

    this.spawnObjects();
  }

  draw() {
    const w = this.canvas.width;
    const h = this.canvas.height;
    const ctx = this.ctx;

    ctx.fillStyle = '#0f051d';
    ctx.fillRect(0, 0, w, h);

    const horizon = h * 0.35;
    const vanishingX = w / 2;

    // レール描画（擬似3Dパースペクティブ）
    ctx.strokeStyle = '#39ff14';
    ctx.lineWidth = 2;
    [-120, -40, 40, 120].forEach(offset => {
      ctx.beginPath();
      ctx.moveTo(vanishingX, horizon);
      ctx.lineTo(vanishingX + offset * 3, h);
      ctx.stroke();
    });

    // 枕木アニメーション
    const tieOffset = (this.distance % 40);
    ctx.strokeStyle = 'rgba(57, 255, 20, 0.4)';
    for (let z = tieOffset; z < 200; z += 30) {
      const y = horizon + (h - horizon) * (z / 200);
      const spread = (z / 200) * 360;
      ctx.beginPath();
      ctx.moveTo(vanishingX - spread, y);
      ctx.lineTo(vanishingX + spread, y);
      ctx.stroke();
    }

    // コイン描画
    this.coins.forEach(c => {
      const depth = (500 - c.z) / 500;
      if (depth <= 0) return;
      const y = horizon + (h - horizon) * depth;
      const laneX = vanishingX + this.lanes[c.lane] * depth * 3.5;
      const size = 6 + depth * 14;

      ctx.fillStyle = '#ffe600';
      ctx.beginPath();
      ctx.arc(laneX, y - 10, size / 2, 0, Math.PI * 2);
      ctx.fill();
    });

    // 障害物（列車/ブロック）描画
    this.obstacles.forEach(o => {
      const depth = (500 - o.z) / 500;
      if (depth <= 0) return;
      const y = horizon + (h - horizon) * depth;
      const laneX = vanishingX + this.lanes[o.lane] * depth * 3.5;
      const ow = 25 * depth * 2.5;
      const oh = 35 * depth * 2.5;

      ctx.fillStyle = '#ff007f';
      ctx.shadowColor = '#ff007f';
      ctx.shadowBlur = 10;
      ctx.fillRect(laneX - ow / 2, y - oh, ow, oh);
      ctx.shadowBlur = 0;
    });

    // プレイヤー描画（無敵時点滅）
    if (this.player.invincibleTimer % 6 < 3) {
      const pLaneX = vanishingX + this.player.currentX * 3.2;
      const py = h - 30 + this.player.y;

      ctx.fillStyle = this.player.invincibleTimer > 0 ? '#ff0055' : '#00f3ff';
      ctx.shadowColor = '#00f3ff';
      ctx.shadowBlur = 15;
      ctx.beginPath();
      ctx.arc(pLaneX, py - 18, 14, 0, Math.PI * 2);
      ctx.fill();
      ctx.fillRect(pLaneX - 10, py - 6, 20, 24);
      ctx.shadowBlur = 0;
    }
  }

  loop() {
    this.update();
    this.draw();
    requestAnimationFrame(this.loop);
  }
}

// --- 3. パーティクル祝賀エフェクト ---
class ParticleCelebration {
  constructor(canvas) {
    this.canvas = canvas;
    this.ctx = canvas.getContext('2d');
    this.particles = [];
    this.resize();
    window.addEventListener('resize', () => this.resize());
    this.loop = this.loop.bind(this);
    requestAnimationFrame(this.loop);
  }

  resize() {
    this.canvas.width = window.innerWidth;
    this.canvas.height = window.innerHeight;
  }

  burst(x, y, count = 30) {
    const colors = ['#ff007f', '#00f3ff', '#ffe600', '#39ff14', '#ffffff'];
    for (let i = 0; i < count; i++) {
      const angle = Math.random() * Math.PI * 2;
      const speed = Math.random() * 8 + 3;
      this.particles.push({
        x,
        y,
        vx: Math.cos(angle) * speed,
        vy: Math.sin(angle) * speed - 3,
        color: colors[Math.floor(Math.random() * colors.length)],
        size: Math.random() * 8 + 4,
        life: 1,
        decay: Math.random() * 0.02 + 0.015,
        isCoin: Math.random() > 0.6
      });
    }
  }

  update() {
    for (let i = this.particles.length - 1; i >= 0; i--) {
      const p = this.particles[i];
      p.x += p.vx;
      p.y += p.vy;
      p.vy += 0.25;
      p.life -= p.decay;
      if (p.life <= 0) {
        this.particles.splice(i, 1);
      }
    }
  }

  draw() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    this.particles.forEach(p => {
      this.ctx.globalAlpha = p.life;
      this.ctx.fillStyle = p.color;
      if (p.isCoin) {
        this.ctx.beginPath();
        this.ctx.arc(p.x, p.y, p.size, 0, Math.PI * 2);
        this.ctx.fill();
      } else {
        this.ctx.fillRect(p.x, p.y, p.size, p.size);
      }
    });
    this.ctx.globalAlpha = 1;
  }

  loop() {
    this.update();
    this.draw();
    requestAnimationFrame(this.loop);
  }
}

// --- 4. コントローラ ＆ ドーパミンインタラクション ---
class DopaController {
  constructor() {
    this.audio = new DopaAudioEngine();
    this.particleCanvas = document.querySelector('.celebration-particles');
    this.particles = new ParticleCelebration(this.particleCanvas);

    const runnerCanvas = document.querySelector('.runner-canvas');
    this.runner = new SubwayRunnerSimulator(
      runnerCanvas,
      () => this.onRunnerCoin(),
      () => this.onRunnerCrash()
    );

    this.boostBtn = document.querySelector('[data-trigger="boost"]');
    this.rerollLink = document.querySelector('[data-action="reroll"]');
    this.scoreOutput = document.querySelector('[data-target="score"]');
    this.dopaMeter = document.querySelector('.dopa-meter');
    this.template = document.querySelector('.pop-dopa-template');

    this.score = 999999999;
    this.hypeWords = ['脳汁確定！', '神引き！', 'ドパドパ！', '失神注意！', '脳が溶ける！', 'チピチピ！', 'チャパチャパ！', '超大吉級！', '大爆発！', '草生える！'];

    this.initEvents();
  }

  spawnFloatingText(x, y, text) {
    if (!this.template) return;
    const clone = this.template.content.cloneNode(true);
    const span = clone.querySelector('.floating-text');
    const displayText = text || (this.hypeWords[Math.floor(Math.random() * this.hypeWords.length)] + ' +' + Math.floor(Math.random() * 50000 + 10000));

    span.textContent = displayText;
    span.style.left = x + 'px';
    span.style.top = y + 'px';
    span.style.setProperty('--rot', (Math.random() * 30 - 15) + 'deg');

    document.body.appendChild(span);
    setTimeout(() => span.remove(), 800);
  }

  triggerShake() {
    document.body.dataset.shake = 'active';
    clearTimeout(this.shakeTimer);
    this.shakeTimer = setTimeout(() => {
      document.body.dataset.shake = 'idle';
    }, 300);
  }

  onBoost(e) {
    this.score += 777777;
    this.scoreOutput.textContent = this.score.toLocaleString();

    const x = e.clientX || window.innerWidth / 2;
    const y = e.clientY || window.innerHeight / 2;

    this.audio.playJackpot();
    this.particles.burst(x, y, 40);
    this.spawnFloatingText(x, y);
    this.triggerShake();
  }

  onRunnerCoin() {
    this.score += 10000;
    this.scoreOutput.textContent = this.score.toLocaleString();
    this.audio.playCoin();

    const canvasRect = this.runner.canvas.getBoundingClientRect();
    const x = canvasRect.left + canvasRect.width / 2;
    const y = canvasRect.top + canvasRect.height / 2;

    this.particles.burst(x, y, 15);
    this.spawnFloatingText(x, y, '🪙 COIN GET! +10,000');
  }

  onRunnerCrash() {
    this.audio.playCrash();
    this.triggerShake();

    const canvasRect = this.runner.canvas.getBoundingClientRect();
    const x = canvasRect.left + canvasRect.width / 2;
    const y = canvasRect.top + canvasRect.height / 2;

    this.spawnFloatingText(x, y, '💥 衝突回避！即時復活');
  }

  initEvents() {
    if (this.boostBtn) {
      this.boostBtn.addEventListener('click', (e) => this.onBoost(e));
    }

    if (this.rerollLink) {
      this.rerollLink.addEventListener('click', () => {
        this.audio.playJackpot();
        this.triggerShake();
      });
    }

    // 初回ロード演出
    window.addEventListener('DOMContentLoaded', () => {
      if (this.boostBtn) {
        const rect = this.boostBtn.getBoundingClientRect();
        this.particles.burst(rect.x + rect.width / 2, rect.y + rect.height / 2, 50);
      }
    });

    // 画面タップ・クリックでコイン音＆小パーティクル（ボタン・コントロール以外）
    window.addEventListener('pointerdown', (e) => {
      if (!e.target.closest('button') && !e.target.closest('a') && !e.target.closest('canvas')) {
        this.audio.playCoin();
        this.particles.burst(e.clientX, e.clientY, 10);
      }
    });
  }
}

// 起動
new DopaController();
