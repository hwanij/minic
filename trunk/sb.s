(0) : enter -1 -1 	-1 -1 -1
(1) : <= a 1 	1 0 2
(2) : if,goto 1 4 	0 1 1
(3) : goto 5 -1 	0 1 -1
(4) : return 1 -1 	1 2 -1
(5) : - a 1 	1 0 2
(6) : param 5 -1 	1 1 -1
(7) : call f -1 	1036 0 -1
(8) : - a 2 	1 0 2
(9) : param 8 -1 	1 1 -1
(10) : call f -1 	1036 0 -1
(11) : + 7 10 	1036 1 1
(12) : return 11 -1 	1036 1 -1
(13) : leave -1 -1 	-1 -1 -1
(14) : enter -1 -1 	-1 -1 -1
(15) : = a 5 	1 0 2
(16) : param a -1 	1 0 -1
(17) : call f -1 	1036 0 -1
(18) : = a 17 	1 0 1
(19) : return 1 -1 	1 2 -1
(20) : leave -1 -1 	-1 -1 -1
