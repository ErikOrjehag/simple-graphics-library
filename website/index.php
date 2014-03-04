<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Simple Graphics Library</title>
    <link rel="shortcut icon" href="img/favicon.png">
    <link rel="stylesheet" href="style/reset.css">
    <link rel="stylesheet" href="style/style.css">
    <link rel="stylesheet" href="style/vs.min.css">
    <script src="script/highlight.pack.js"></script>
    <script>hljs.initHighlightingOnLoad();</script>
  </head>
  <body>
    <?php include_once("script/analyticstracking.php"); ?>
    <header>
      <div class="wrap">
        <?php include_once("sections/header.html"); ?>
      </div>
    </header>
    <?php
      $sections = array("getStarted", "drawing", "setters", "getters", "transformation", "accessGDI");
      for($i = 0; $i < count($sections); $i++) {
        if($i != 0) echo "<div class='stripes'></div>";
        echo "<section class='wrap'>";
        include_once("sections/$sections[$i].html");
        echo "</section>";
      }
    ?>
    <footer>
      Created by Erik Örjehag <i>sgl@orjehag.se</i><br>
      and the community via <a href="https://github.com/ErikOrjehag/simple-graphics-library">GitHub</a><br>
      <br>
      &copy; <?php echo Date('Y'); ?> SGL
    </footer>
  </body>
</html>