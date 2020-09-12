#include "create_customer.h"


//code to dynamically generate a random customer and circumstance

int get_random_age(int i_age_range)
{
  int i_age = 0;

  if (i_age_range == c_age_range_kid) i_age = random_range(7,11);
  if (i_age_range == c_age_range_teen) i_age = random_range(15,19);
  if (i_age_range == c_age_range_adult) i_age = random_range(28,40);
  if (i_age_range == c_age_range_old) i_age = random_range(60,99);
  if (i_age == 0) log_error("get_random_age: error, i_age_range invalid");
  return i_age;

}



bool different_sex(LPREQUEST lpReq)
{
   	 online_struct *p = p_on[lpReq->i_on_index];

	 if (p->status.b_customer_sex_male == p->status.b_deceased_sex_male) return false;
	 else return true;


}

void define_relationship_with_the_deceased(LPREQUEST lpReq)
{
	online_struct *p = p_on[lpReq->i_on_index];
	
	int e_customer_age_range = get_range_from_age(p->status.i_customer_age);
	int e_deceased_age_range = get_range_from_age(p->status.i_deceased_age);	
	
	if (e_customer_age_range == c_age_range_kid)
	{
		//customer is a child
		if (e_deceased_age_range == c_age_range_kid)
		{
			//deceased is a child
			p->status.e_relation = ::c_relation_sibling; 
		}
		if (e_deceased_age_range == c_age_range_teen)
		{
			//deceased is a teen
			p->status.e_relation = ::c_relation_sibling; 
		}
		if (e_deceased_age_range == c_age_range_adult)
		{
			//deceased is an adult
			p->status.e_relation = ::c_relation_parent; 
		}
		if (e_deceased_age_range == c_age_range_old)
		{
			//deceased is old
			p->status.e_relation = ::c_relation_grand_parent; 
		}
		
	}
	
	if (e_customer_age_range == c_age_range_teen)
	{
		//customer is a teen
		if (e_deceased_age_range == c_age_range_kid)
		{
			//deceased is a child
			p->status.e_relation = ::c_relation_sibling; 
		}
		if (e_deceased_age_range == c_age_range_teen)
		{
			//deceased is a teen
			int i_random =  random(3);
			p->status.e_relation = ::c_relation_sibling; 
			
			if (i_random == 1)
			{
				if (different_sex(lpReq))
					p->status.e_relation = ::c_relation_lover;		  
			}
		}
		if (e_deceased_age_range == c_age_range_adult)
		{
			//deceased is an adult
			int i_random = random(3);
			//default
			p->status.e_relation = ::c_relation_parent;
			
			if (i_random == 1) if (different_sex(lpReq)) p->status.e_relation = ::c_relation_lover;
			
		}
		if (e_deceased_age_range == c_age_range_old)
		{
			//deceased is old
			p->status.e_relation = ::c_relation_grand_parent; 
		}
		
	}
	
    if (e_customer_age_range == c_age_range_adult)
	{
		//customer is an adult
		if (e_deceased_age_range == c_age_range_kid)
		{
			//deceased is a child
			p->status.e_relation = ::c_relation_child; 
		}
		if (e_deceased_age_range == c_age_range_teen)
		{
			//deceased is a teen
			int i_random = random(3);
			//default
			p->status.e_relation = ::c_relation_child; 
			if (i_random == 1) if (different_sex(lpReq))
				p->status.e_relation = ::c_relation_lover;		  
		}
		if (e_deceased_age_range == c_age_range_adult)
		{
			//deceased is an adult
			int i_random = random(6);
			//set default
			p->status.e_relation = ::c_relation_sibling;
			
			if (i_random > 3)
			{
				if (different_sex(lpReq))
					p->status.e_relation = ::c_relation_spouse;
				{
					if (i_random == 1)
					{
						if (different_sex(lpReq))
							p->status.e_relation = ::c_relation_lover; else
							p->status.e_relation = ::c_relation_friend;
						
					}
					
				}
			}
		}
			if (e_deceased_age_range == c_age_range_old)
			{
				//deceased is old
				int i_random = random(6);
				//set default
				p->status.e_relation = ::c_relation_parent; 
				if (i_random == 1) if (different_sex(lpReq)) p->status.e_relation = ::c_relation_lover;
				if (i_random == 2) if (different_sex(lpReq)) p->status.e_relation = ::c_relation_spouse;
			}
			
		}
			
			if (e_customer_age_range == c_age_range_old)
			{
				//customer is an adult
				if (e_deceased_age_range == c_age_range_kid)
				{
					//deceased is a child
					p->status.e_relation = ::c_relation_grand_child; 
				}
				if (e_deceased_age_range == c_age_range_teen)
				{
					//deceased is a teen
					int i_random = random(10);
					//default
					p->status.e_relation = ::c_relation_grand_child;		  
					if (i_random == 1) if (different_sex(lpReq))
						p->status.e_relation = ::c_relation_lover;
					
				}
				if (e_deceased_age_range == c_age_range_adult)
				{
					//deceased is an adult
					int i_random = random(10);
					//set default
					p->status.e_relation = ::c_relation_child;
					if (i_random == 1) if (different_sex(lpReq))
						p->status.e_relation = ::c_relation_lover;
					if (i_random == 2) if (different_sex(lpReq))
						p->status.e_relation = ::c_relation_spouse;
					
				}
				if (e_deceased_age_range == c_age_range_old)
				{
					//deceased is old
					int i_random = random(6);
					//set default
					if (different_sex(lpReq))	p->status.e_relation = ::c_relation_spouse;
					else
						p->status.e_relation = ::c_relation_sibling; 
					if (i_random == 1) if (different_sex(lpReq)) p->status.e_relation = ::c_relation_lover;
					if (i_random == 2) p->status.e_relation = ::c_relation_sibling;
				}
				
			}
			
			
		}
		

int get_desired_casket(LPREQUEST lpReq)
{
   int i_casket;
	online_struct *p = p_on[lpReq->i_on_index];
    again:	
	i_casket = random(3); //hack for now, do real math later
	p->status.e_desired_casket = i_casket;

	if (p->get_price_int() > p->status.i_money_to_spend) goto again;
	return i_casket;
}


int figure_money_to_spend(LPREQUEST lpReq)
{
	int i_amount = 0;
	online_struct *p = p_on[lpReq->i_on_index];
	switch(p->status.e_income)
	{
	case c_income_poor:
		i_amount = random_range(C_COFFIN_WOOD_PRICE+1, 2800);
		break;
   	case c_income_average:
		i_amount = random_range(1900, 5400);
		break;

	case c_income_rich:
		i_amount = random_range(3700, 7530);
		break;

	case c_income_super_rich:
		i_amount = random_range(4600, 18500);
		break;

	};
 
   return i_amount;
}


void create_random_customer(LPREQUEST lpReq)
{
  online_struct *p = p_on[lpReq->i_on_index];

  
  //clear status
  memset(&p->status,0,sizeof(communication_status));
 p->a_options.RemoveAll();

  //male or female customer?
  if (random(2) == 0)
  p->status.b_customer_sex_male = true;
  else
  p->status.b_customer_sex_male = false;


  //get age of customer
   
  //actually, we don't want a kid to be a customer, so we'll make sure
  //the random can't be 0
  p->status.i_customer_age = get_random_age(1+random(c_max_age_range-1));
   
 
  //get information on the deceased

  //male or female?
  if (random(2) == 0)
  p->status.b_deceased_sex_male = true;
  else
  p->status.b_deceased_sex_male = false;

  //get age of the deceased
  p->status.i_deceased_age = get_random_age(random(c_max_age_range));

  //define relationship to the deceased.  He was my...
  define_relationship_with_the_deceased(lpReq);

  //set random income level
  p->status.e_big_upgrade = ::C_UPGRADE_NONE;
  p->status.e_income = random(4); 
  p->status.i_money_to_spend = figure_money_to_spend(lpReq);
  if (random(2) == 1)
  p->status.b_loved = true; else p->status.b_loved = false;
  p->status.f_mood = frandom(10.0f);
  p->status.f_cheapness = frandom(10.0f);
  //let's see how he was killed
  p->status.e_death_object = random(::C_MAX_KILL_TEXTS);
  p->status.e_death_type = random(::C_MAX_KILL_METHOD_TEXTS);

  p->status.e_desired_casket = get_desired_casket(lpReq);
  p->status.b_customer_active = true;
}
