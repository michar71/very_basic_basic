SUB SUBR X,Y,Z				# SUBROUTINE
	LOCAL	N				# PROTECT VARS
	N=	10					# MODIFY LOCAL
	FORMAT	"SUBR: %,%,%", X,Y,Z		# PRINT PARAMS
	FORMAT	"SUBR: OK!"		# DONE
	RETURN					# PLAIN RETURN
	FORMAT	"S: RETURN DID NOT RETURN!"	# IT BETTER HAD!
END SUB						#

SUB F	X,Y					# FUNCTION
	RETURN	X+Y				# USE VALUES
	FORMAT	"F: RETURN DID NOT RETURN!"	# IT BETTER HAD!
END SUB						#

N=	400						# ASSIGNMENT
X=	100						# SET THESE GLOBALS
Y=	200						# ...
Z=	300						# ...
SUBR	1,2,3				# CALL SUBROUTINE
FORMAT	"X,Y,Z,N: %,%,%,%", X,Y,Z,N			# CHECK RESTORED

FORMAT	"VERSION: %", VERSION()	# CALL FUNCTION

L = F(120,80)			            # CALL FUNCTION
FORMAT	"F(100,200): %", F(100,L)	# CALL FUNCTION

I=	0						#
WHILE	I<5					# SIMPLE WHILE
	FORMAT	"WHILE I:%",I	#
	I=I+1					#
END WHILE					#

SUB TESTLOOP				# LOCAL I LOOP
	LOCAL I					#
	FOR I=0 TO 3			#
	END FOR					#
END SUB						#

FOR	I=0 TO 5				# LOOPS DON'T INTERFERE
	TESTLOOP				# TESTLOOP USES I
END FOR						#

FOR	I=0 TO 3				# NESTED LOOPS
FOR	J=10 TO 12				#
	FORMAT	"% %", I,J		#
END FOR						#
END FOR						#

SUB	TESTESCAPE				#
	FOR	I=0 TO 10			# LEAVES 10 ON THE STACK
		RETURN				# POP "FOR" MAX
	END FOR					# NORMALY POPPED AFTER HERE
END SUB						#
TESTESCAPE					# CRASH IF TEMPS NOT HANDLED

IF	0						# FAILED CLAUSE
	FORMAT	"NO"			#
ELSE IF 1<2					# SUCCESSFUL CLAUSE
	FORMAT	"YES!"			# EXECUTE AND SKIP ELSE
ELSE						#
	FORMAT "WTF"			#
END IF						#

IF	1 THEN FORMAT "IN-LINE OK"		# IN-LINE IF
IF	0 THEN FORMAT "IN-LINE BAD"		# IN-LINE IF

DIM	A(10)					# ALLOCATE FOR A
FOR	I=1 TO UBOUND(A)+1		# INITIALISE
	A(I)=	I*10			#
END FOR						#
FORMAT	"% % %",A(1),A(10),UBOUND(A)		# PRINT


#FORMAT	"RESUMED BOUND: %", A(0)	# USE "RESUME 1" ON FAULT
#FORMAT "RESUMED DIV: %", 1/0		# "RESUME 2" ON FAULT
#BREAK								# IMMEDIATE MODE
FORMAT	"DONE."
