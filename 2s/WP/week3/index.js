const express = require('express');
const path = require('path');
const app = express();

//app.set('view engine', 'ejs');
app.use("/public", express.static(path.join(__dirname + "/public")));

app.get("/", (req, res) => {
  res.send('Hello, World\n');
});

app.get("/english", (req, res) => {
  res.send('Good Morning\n');
});

app.get("/french", (req, res) => {
  res.send('Bonjour\n');
});

app.get("/german", (req, res) => {
  res.send('Guten Morgen\n');
});

app.use(function (req, res, next) {
  res.status(404).send('ページが見つかりません');
});

app.listen(8080, () => console.log("Example app listening on port 8080!"));
