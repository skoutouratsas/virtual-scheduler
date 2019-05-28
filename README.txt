Stergios Koutouratsas 2162 skoutouratsas@uth.gr
Iwannis Karamporos 2264 ikaramporos@uth.gr
Eustathios Tsitsopoylos 2283 etsitsopoulos@uth.gr

Paratiriseis:

	test1(non-interactive):
		SJF(exp_burst): exoume mia diergasia h opoia ksekinaei na trexei monh ths anaptussontas etsi megalo exp_burst tin stigmi poy kanoun spawn oi alles 2.
				me apotelesma oi 2 nees diergasies na monopoloun ton epeksergasti epeidh enalassontai kai to exp_burst den 8a megalwsei arketa oste 
				na kanoun yield thn cpu. Uparxei kindinos limoktonias an sinexisoun na ftanoun kainouries!

		 SJF(goodness):o sunupologismos tou poso perimenei mia diergasia gia thn cpu lunei auto provlima ka8ws to baros tou waiting_in_rq kapoia stigmi 8a kanei tin 
				arxiki na exei mikrotero goodness apo tis kainouries kai ara 8a lavei thn cpu.

	 test2(interactive):
	     SJF(exp_burst): exoume paromoio provlima me to test1 opou mia diergasia pithanws na treksei moni tis dioti oi alles einai I/O blocked kai na anaptiksei megalo exp_burst.
			     an tuxei na ksipnisoun oles mazi kai na antagwnistoun gia tin cpu auth tha einai se meionektiki thesi kai tha argisei na parei tin cpu. meiwnetai simantika 
		 	     i antapokrisimotita tis.

	      SJF(goodness): o sunupologismos tou poso perimenei mia diergasia gia thn cpu beltiwnei tin antapokrisimotita.


	 test3(mixed):
       SJF(exp_burst): edw mporoume na limoktonisoume tin interactive diergasia h opoia ksekinaei monh tis me 2 non-interactive oi opoies den paraxwroun ton epeksergasti gia ta mikra diastimata
		       pou tin xreiazetai. pali exoume thema antapokrisimotitas.

        SJF(goodness): edw den uparxei periptwsi limoktonias alla oute einai kai idanikos o algorithmos kathws oi non-interactive den dinoun protereotita gia tous short upologismous tis 
		       interactive.

				
