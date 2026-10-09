const express = require("express");
const path = require('path');
const app = express();

//app.set('view engine', 'ejs');
app.use("/public", express.static(path.join(__dirname + "/public")));

app.get("/", (req, res) => {
  res.write('Hello, World\n');
});

app.use(function(req, res, next) {
  res.status(404).send('ページが見つかりません');
});

app.listen(8080, () => console.log("Example app listening on port 8080!"));