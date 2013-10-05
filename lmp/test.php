<html>
<head>
<script type="text/javascript" src="js/test.js"></script>
</head>
<body>
	<h1>test 00</h1>
	<?php
	$con = mysql_connect("localhost", "l1038416ro", "xyz");
	if (!$con) {
		die('Could not connect: ' . mysql_error());
	}

	if (mysq_query("CREATE DATABASE my_db", $con)) {
		echo "Database created";
	} else {
		echo "Error creating database: " . mysql_error();
	}
	mysql_close($con);
	?>

</body>
