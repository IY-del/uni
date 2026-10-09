const express = require("express");
const path = require('path');
const { getOmikuji, getComment } = require('./omikuji');
const app = express();

app.set('view engine', 'ejs');
app.use("/public", express.static(path.join(__dirname + "/public")));
app.use(express.urlencoded({ extended: true }));

app.get("/hello1", (req, res) => {
  const message1 = "Hello world";
  const message2 = "Bon jour";
  res.render('show', { greet1: message1, greet2: message2 });
});

app.get("/hello2", (req, res) => {
  res.render('show', { greet1: "Hello world", greet2: "Bon jour" });
});

app.get("/icon", (req, res) => {
  res.render('icon', { filename: "./public/Apple_logo_black.svg", alt: "Apple Logo" });
});

app.get("/omikuji1", (req, res) => {
  const luck = getOmikuji();
  res.send('あなたの運勢は' + luck + 'です');
});

app.get("/omikuji2", (req, res) => {
  const luck = getOmikuji();
  res.render('omikuji2', { result: luck });
});

app.get("/omikuji3", (req, res) => {
  const luck = getOmikuji();
  const comment = getComment(luck);
  res.render('omikuji3', { result: luck, comment: comment });
});

app.get("/english", (req, res) => {
  res.render('greeting', { message: 'Good Morning\n' });
});

app.get("/french", (req, res) => {
  res.render('greeting', { message: 'Bonjour\n' });
});

app.get("/german", (req, res) => {
  res.render('greeting', { message: 'Guten Morgen\n' });
});

app.listen(8080, () => console.log("Example app listening on port 8080!"));
