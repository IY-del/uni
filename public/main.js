/**
 * DopamineManager - 脳汁とタイパを司るフロントエンドOOPモジュール
 */
class DopamineManager {
  constructor() {
    this.score = 0;
    this.capacity = 100;
    this.meterFill = document.querySelector(".meter-fill");
    this.scoreDisplay = document.querySelector("[data-role='score-display']");
    this.injectButton = document.querySelector("[data-action='inject']");
    this.gachaButton = document.querySelector("[data-action='gacha']");
    this.container = document.querySelector("main");
    this.effectLayer = document.querySelector(".effect-layer");
    this.templateParticle = document.querySelector("template[data-type='particle']");
    this.templateLog = document.querySelector("template[data-type='log-entry']");
    this.logBox = document.querySelector(".log-box");

    this.init();
  }

  init() {
    if (this.injectButton) {
      this.injectButton.addEventListener("click", (e) => this.handleInject(e));
    }
    if (this.gachaButton) {
      this.gachaButton.addEventListener("click", () => this.handleGacha());
    }
  }

  handleInject(event) {
    this.score += 500;
    const currentPercent = Math.min(100, Math.floor((this.score % 5000) / 50));

    if (this.scoreDisplay) {
      this.scoreDisplay.textContent = this.score.toLocaleString();
    }
    if (this.meterFill) {
      this.meterFill.style.width = `${Math.max(10, currentPercent)}%`;
    }

    // ドパガキ演出（状態属性の更新）
    if (this.container) {
      this.container.dataset.rush = "active";
      clearTimeout(this.rushTimer);
      this.rushTimer = setTimeout(() => {
        this.container.dataset.rush = "idle";
      }, 350);
    }

    this.spawnParticle(event.clientX, event.clientY);
  }

  spawnParticle(x, y) {
    if (!this.templateParticle || !this.effectLayer) return;

    const clone = this.templateParticle.content.cloneNode(true);
    const particle = clone.querySelector(".floating-text");
    if (!particle) return;

    const messages = ["+500 脳汁!", "⚡️神引き!", "脳汁プッシャー!", "🔥タイパ最高!", "脳死タップ!!"];
    const randomMsg = messages[Math.floor(Math.random() * messages.length)];

    particle.textContent = randomMsg;
    particle.style.left = `${Math.max(10, (x || window.innerWidth / 2) - 60)}px`;
    particle.style.top = `${Math.max(10, (y || window.innerHeight / 2) - 30)}px`;

    this.effectLayer.appendChild(particle);

    setTimeout(() => {
      particle.remove();
    }, 750);
  }

  handleGacha() {
    if (!this.templateLog || !this.logBox) return;

    const results = [
      "🌈【SSR】3秒でわかる人生の結論（脳汁+10000%）",
      "⚡️【SR】サブウェイサーファーズ倍速動画（集中力+3秒）",
      "🔥【SR】ショート動画スワイプ無限機関（睡眠時間-2時間）",
      "💥【R】エナジードリンク一気飲み（心拍数急上昇）",
      "💣【N】1分以上の長文（即スキップ）"
    ];

    const pick = results[Math.floor(Math.random() * results.length)];
    const clone = this.templateLog.content.cloneNode(true);
    const entry = clone.querySelector(".log-entry");
    if (entry) {
      entry.textContent = `[${new Date().toLocaleTimeString()}] ${pick}`;
      this.logBox.prepend(entry);
    }
  }
}

document.addEventListener("DOMContentLoaded", () => {
  new DopamineManager();
});
