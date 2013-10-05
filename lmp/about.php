<html>
	<head>
		<title>test php</title>
		<META http-equiv=Content-Type content="text/html; charset=UTF-8">
		<link href="fav.png" rel="shortcut icon">
		<link rel="stylesheet" type="text/css" href="css/style.css">
	</head>
	<body>
		<div id="header">
			<?php include("inc/header.php"); ?>
		</div>
		<div id="nav">
			<?php include("inc/nav.php"); ?>
		</div>
		<div id="content">
			<div id="#">
				TEST
			</div>
			<div id="copyright">
				<?php
					$file = fopen("copyright.txt", "r")
					or exit("Unable to open file!");
					while (!feof($file)) {
						echo fgets($file) . "<br />";
					}
					fclose($file);
				?>
			</div>
		</div>
		<div id="footer">
			<?php include("inc/footer.php"); ?>
		</div>
	</body>
</html>
