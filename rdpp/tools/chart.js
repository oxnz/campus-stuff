function draw() {
	var data = new Array(
		2,6,10,12,2685,12953,16370,16194,15745,15549,
		15570,15694,15581,15538,15181,14955,15149,15003,14691,14351,
		14114,13847,13759,13642,13418,13319,13094,12867,12735,12533,
		12321,12447,12082,12177,12130,12082,11912,11664,11598,11310,
		11255,11172,11079,10933,10778,10908,10662,10550,10588,10282,
		10080,10022,10202,10267,10155,10140,10109,10021,9929,9585,
		9376,9368,9435,9371,9317,9370,9306,9120,9219,9222,
		9255,9245,9307,9257,9311,9113,8919,8937,8905,8829,
		8820,8869,8954,8776,8857,8969,9094,8952,9248,9267,
		9333,9367,9441,9396,9592,9743,9845,9957,10090,10280,
		10579,10778,10999,11265,11297,11336,11644,11924,12282,12707,
		12926,13216,13428,13571,13924,14048,14323,14404,14844,14845,
		15049,15251,15434,15645,16016,16436,16442,16617,16486,16851,
		16814,16913,16975,16915,16818,17036,17391,17844,17746,17625,
		17593,17657,17879,17598,17236,17467,17306,17337,17256,17099,
		16804,16403,16584,16358,16422,16037,15720,15717,15706,15480,
		15660,15659,15492,15289,15070,15155,15181,15249,15271,15016,
		14953,14860,14990,15202,15300,15638,15405,15630,15909,16039,
		16125,16280,16158,16180,15986,15983,16038,16041,16037,16147,
		15988,16356,16330,16497,16759,17075,17133,17084,17187,17253,
		17340,17762,17637,17877,18008,18184,18337,18450,18444,18589,
		18531,18890,19090,19194,19202,19351,19656,19608,19477,19451,
		19262,19511,19565,19586,19714,19452,19384,19883,19438,19203,
		19169,18888,18961,19232,19158,19362,18963,18729,18885,19258,
		19290,19126,19737,19766,20113,19950,20175,19960,19945,19814,
		19934,20243,19770,19650,19454,19678,19647,19061,19131,18634,
		18664,18416,18652,18596,18076,17942,17476,16732,16590,16646,
		16589,16727,16770,16436,16499,16669,17077,16913,16951,16510,
		16538,16703,16613,16772,16702,16686,16498,16584,16883,16871,
		16798,17014,16914,17245,17002,17189,17463,17499,17253,17497,
		17427,17545,17378,17635,17619,17810,17971,17903,17934,17841,
		17898,17677,17822,17303,17483,17591,17991,17789,17958,18003,
		17686,17533,17367,17318,17295,17533,17703,17587,17293,17224,
		17464,17439,17126,16808,16864,16765,16632,16851,16775,16747,
		16719,16690,16651,16483,15934,15962,15500,15384,15257,15014,
		14516,14597,14458,14146,14179,14071,13862,13576,13436,13452,
		13343,13475,13368,13621,13464,13570,13454,13229,13664,13620,
		13637,13745,13474,13684,14044,14097,14332,14330,14704,14630,
		14708,14790,14875,15404,15619,15857,15998,16186,16514,16777,
		16816,16823,17214,17389,17594,17733,17735,17943,17786,17769,
		17603,17583,17425,17420,17455,17495,17183,16944,16791,16648,
		16997,17058,17148,17035,16869,16737,16368,16087,16053,15849,
		15835,15701,15392,15353,14999,10766,11664,13752,14482,14457,
		14548,14510,14391,14419,14287,14301,14313,14575,15063,15180,
		14713,14764,15028,14902,14682,14755,14645,14723,14957,15140,
		15113,15099,15347,15551,15833,15922,15970,16191,16288,16211,
		16600,16495,16544,16947,16550,16382,16386,16435,16332,16299,
		16617,16425,16551,17236,17465,17321,17189,17110,17356,17117
	);
	var ts = 3; // 3 min
	var tcnt = 24*60/ts; // ts count
	var ccnt = 20000; // max car count
	var width = 960;
	var dx = 960*ts/(24*60);
	var height = 400;
	var dy = 400/ccnt;
    var chart = document.getElementById('chart');
    var ctx = chart.getContext('2d');
    ctx.fillStyle = '#00FF00';
	for (var i = 0; i < tcnt; ++i) {
        ctx.fillRect(i*dx, height-dy*data[i], 2, dy*data[i]);
	}
    var dx = 960/24;
    ctx.fillStyle = '#FF0000';
    ctx.font = "10px Arial";
    for (var i = 0; i < 25; ++i) {
        ctx.fillRect(i*dx, 0, 1, height);
        ctx.fillText(i, i*dx+dx/3, 10);
    }
}