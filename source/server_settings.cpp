#include "server_settings.h"
#include "security.h"


void build_server_settings_htm()
{
     CString cst_html;
	 CString cst_temp;

     unlink("public\\pages\\server.htm");
     append_file("public\\pages\\server_start.htm", "public\\pages\\server.htm");

	 cst_temp.Format("`$Admin email: `w<a href=\"mailto:%s\">%s</a><br>", glo.st_admin_email, glo.st_admin_email);
	 cst_html += cst_temp;

	 cst_temp.Format("`$Turns per day: `w%d<br>", glo.i_turns_per_day);
	 cst_html += cst_temp;
     cst_temp.Format("`$Yang's per day: `w%d<br>", glo.i_yangs_per_day);
	 cst_html += cst_temp;
     cst_temp.Format("`$Player visits per day: `w%d<br>", glo.i_visits_per_day);
	 cst_html += cst_temp;
     cst_temp.Format("`$Risk win/lose amount: `w%d<br>", glo.i_risk);
	 cst_html += cst_temp;
     cst_temp.Format("`$Top player BP bonus: `w%d<br>", glo.i_bonus_exp);
	 cst_html += cst_temp;
     cst_temp.Format("`$Top player luck bonus: `w%d<br>", glo.i_bonus_luck);
	 cst_html += cst_temp;
     
	 cst_temp.Format("`$Amount of tags players can receive by /give per day: `w%d<br>", glo.i_tags_recievable_per_day);
	 cst_html += cst_temp;


	if (glo.i_name_change_cost > 0)
	 cst_temp.Format("`$Cost to change name: `w%d<br>", glo.i_name_change_cost);
	else
		cst_temp.Format("`$Cost to change name: `wDisabled<br>");

	 cst_html += cst_temp;


	 cst_temp.Format("<br>");
	 cst_html += cst_temp;


     cst_temp.Format("`$Starting Hitpoints: `w%d<br>", glo.i_starting_hp);
	 cst_html += cst_temp;
     cst_temp.Format("`$Starting Strength: `w%d<br>", glo.i_starting_strength);
	 cst_html += cst_temp;
     cst_temp.Format("`$Starting Luck: `w%d<br>", glo.i_starting_luck);
	 cst_html += cst_temp;
     if (cls_pro.b_validated && cls_pro.i_max_players < 11)
     {
     cst_temp.Format("`$Note: `4This server is a demo server and only allows %d players max.", cls_pro.i_max_players);
	 cst_html += cst_temp;
         
     }
	 

	 seth_ansi(&cst_html);

	 add_text((char*)(LPCSTR)cst_html, "public\\pages\\server.htm");

     append_file("public\\pages\\server_end.htm", "public\\pages\\server.htm");

}