<html>
	<?php
		header("Last-Modified: " .dmdate("D, d M Y H:i:s") . "GMT");
		header("Cache-Control: no-cache, must-revalidate, no-store, no-cache, must-revalidate, post-check=0, pre-check=0")
		header("Pragma: no-cache")
	?>
	<head>
		<title>test php</title>
		<META http-equiv=Content-Type content="text/html; charset=UTF-8">
		<link href="fav.png" rel="shortcut icon">
		<link rel="stylesheet" type="text/css" href="css/style.css">
	</head>
	<body>
		<ul>
			<li><a href="test.php">test.php</a></li>
			<li><a href="test.php">about.php</a></li>
			<li><a href="test.php">edit.php</a></li>
			<li><a href="test.php">welcome.php</a></li>
			<li><a href="test.php">test.php</a></li>
		</ul>
		<h1>test php</h1>
		<?php
			$str1="Hello";
			$str2="World!";
			echo $str1 . " " . $str2 . "Is this JP?";
		?>
		<hr />
		<?php
			$d = date("D");
			if ($d == "Fri")
				echo "Have a nice weekend!";
			elseif ($d == "Sun")
				echo "Have a nice Sunday!";
			else
				echo "Have a nice day!";
		?>
		<hr />
		<?php
			$arr = array("one", "two", "three");
			foreach ($arr as $value) {
				echo "<h4>Value: " . $value . "</h4><br />";
			}
		?>
		<h3>END</h3>
	</body>
</html>
