
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
			<div id="edit">
				<form action="upload_file.php" method="post" enctype="multipart/form-data">
				<label for="file">File:</label>
				<input type="file" name="file" id="file"></input>
				<br />
				<input type="submit" name="submit" value="Submit"></input>
				</form>
			</div>
		</div>
		<div id="footer">
			<?php include("inc/footer.php"); ?>
		</div>
	</body>
</html>
