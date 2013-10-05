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
			Welcome <?php echo $_POST["name"]; ?> . <br />
			Your password is : <?php echo $_POST["password"]; ?> . <br />
		</div>
		<div id="footer">
			<?php include("inc/footer.php"); ?>
		</div>
	</body>
</html>
