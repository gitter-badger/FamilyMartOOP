#include <string>
#include <fstream>
#include "model.h"

namespace event
{
	class eventmanager
	{
		std::vector<model::shop> subscribers;
	public:
		eventmanager()
		{
			publish("init_item","item.txt");
			publish("init_shop","shop.txt");
		}

		void listen()
		{
			std::cout<<"\n>> ";
			std::string command;
			std::getline(std::cin,command);
			while (command!="close")
			{
				int div = command.find(" ");
				if (div!=-1){
					publish(command.substr(0,div),command.substr(div+1,command.length()));
				}else
				{
					publish(command);
				}
				std::cout<<"\n>> ";
				std::getline(std::cin,command);
			}
		}
	private:
		void publish(std::string command,std::string file="")
		{
			if (command.compare("init_item")==0)
			{
				std::cout<<">> init[item]"<< std::endl;
				std::ifstream is(file);
				std::string line;

				// process header
				std::getline(is, line);

				// prepare shop
				model::shop shop("SHOP1");


				// skip second and third line
				std::getline(is, line);
				std::getline(is, line);
				;
				// process content
				while (std::getline(is, line))
				{
					int div[4]={0,0,0,0};
					for (int i = 1; i < 4; i++)
					{
						div[i]=line.find_first_of(" \t",div[i-1]+1);
					}
					// prepare item
					std::string name = line.substr(0,div[1]),
						id = line.substr(div[1]+1,div[2]-div[1]-1),
						price = line.substr(div[2]+1,div[3]-div[2]-1),
						life = line.substr(div[3]+1,line.length()-div[3]-1);
					model::item item(name, std::stoi(id),std::stod(price), std::stoi(life));
					shop.prepare_item(item);
				}

				subscribers.push_back(shop);
			}

			if (command.compare("init_shop")==0)
			{
				std::cout<<">> init[shop]"<< std::endl;
				std::ifstream is(file);
				std::string line;

				// process header
				std::getline(is, line);

				std::string event;
				int count=0;
				std::string subscriber;

				int div1=line.find_first_of(" ");
				subscriber = line.substr(0,div1);
				count = stoi(line.substr(div1+1,line.length()));

				// prepare shop
				std::vector<model::shop>::iterator it;
				for (it = subscribers.begin() ; it != subscribers.end(); ++it)
				{
					if (it->shop_name==subscriber)
					{
						break;
					}
				}

				// skip second and third line
				std::getline(is, line);
				std::getline(is, line);

				// process content
				while (count-->0)
				{
					std::getline(is, line);
					int div=line.find_first_of(" \t");

					// prepare item
					std::string name = line.substr(0,div),
						product_date = line.substr(div+1,line.length()-div-1);
					it->purchase_item(name,product_date);
				}
			}
			if (command.compare("purchase")==0)
			{
				std::ifstream is(file);
				std::string line;

				// process header
				std::getline(is, line);

				std::string event;
				int count=0;
				std::string subscriber;

				int div1=line.find_first_of(" \t"),
					div2=line.find_first_of(" \t",div1+1),
					div3=line.find_first_of(" \t",div2+1);
				event = line.substr(0,div1);
				count = stoi(line.substr(div1+1,div2-div1-1));
				subscriber=line.substr(div2+1,div3-div2-1);

				// prepare shop
				std::vector<model::shop>::iterator it;
				for (it = subscribers.begin() ; it != subscribers.end(); ++it)
				{
					if (it->shop_name==subscriber)
					{
						break;
					}
				}

				// skip second and third line
				std::getline(is, line);
				std::getline(is, line);

				// process content
				while (count-->0)
				{
					std::getline(is, line);
					int div=line.find_first_of(" \t");

					// prepare item
					std::string name = line.substr(0,div),
						product_date = line.substr(div+1,line.length()-div-1);
					it->purchase_item(name,product_date);
				}
			}
			if (command.compare("sell")==0)
			{
				std::ifstream is(file);
				std::string line;

				while (std::getline(is, line))
				{
					// process header
					std::string event;
					double discount=1;
					std::string subscriber;

					int div1=line.find_first_of(" \t"),
						div2=line.find_first_of(" \t",div1+1);
					event = line.substr(0,div1);
					if (div2!=-1)
					{
						discount=stod(line.substr(div1+1,div2-div1-1));
						subscriber=line.substr(div2+1,line.length()-div2-1);
					}
					else
					{
						subscriber=line.substr(div1+1,line.length()-div1-1);
					}

					// prepare shop
					std::vector<model::shop>::iterator it;
					for (it = subscribers.begin() ; it != subscribers.end(); ++it)
					{
						if (it->shop_name==subscriber)
						{
							break;
						}
					}

					it->add_sell_record_head(event);
					// process content
					while (std::getline(is, line) && line!="")
					{
						// prepare item
						std::string name = line;
						it->sell_item(name,discount);
					}
					it->add_sell_record_head("\n");
					if (it->sales>=55 && subscribers.size()==1)
					{
						model::shop shop("SHOP2");
						subscribers.push_back(shop);
					}
				}

			}
			if (command.compare("show_sale_amount")==0)
			{
				for (std::vector<model::shop>::iterator it = subscribers.begin() ; it != subscribers.end(); ++it)
				{
					std::cout << it->shop_name<<" : "<<it->sales<< std::endl;
				}
			}
			if (command.compare("show_all_shop_name")==0)
			{
				for (std::vector<model::shop>::iterator it = subscribers.begin() ; it != subscribers.end(); ++it)
				{
					std::cout << it->shop_name<<" ";
				}
				std::cout<< std::endl;
			}
			if (command.compare("show_all_sale_records")==0)
			{
				for (std::vector<model::shop>::iterator it = subscribers.begin() ; it != subscribers.end(); ++it)
				{
					if (it->shop_name==file)
					{
						std::cout << it->get_sell_record();
						return;
					}

				}
				std::cout<< "This shop does not exist!"<< std::endl;
			}
		}
	};

}