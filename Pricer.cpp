#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;


typedef struct Order
{
  int id;
  char side;
  double price;
  int size;
} Order;

typedef std::pair<double, int> Pricepair; //<Price, Order-ID>
typedef std::pair<int,std::string> Orderpair; //<Size, Side>
typedef std::vector<Pricepair> Pricevector;
typedef std::map<int,Order> OrderMap;


//Method to split line by delimiter and replace input vector with new vector
std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    elems.clear();
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
  
void ordered_insert(Pricevector &vect, Order order)
{
  auto lower_bound_cmp =  [](Pricepair const & x, double d) -> bool
    { return x.first > d; };
  if (order.side == 'B')
  {
    auto it = std::lower_bound( vect.begin(), vect.end(), order.price, lower_bound_cmp); // find proper position in descending order
    vect.insert( it, Pricepair(order.price,order.id)); // insert before iterator it
  }
  else if (order.side == 'S')
  {
    //TO DO  
  }
  else
    cout << "Error invalid input side in ordered_insert" << endl;
}
  
  //Function to print bid_expense, ask_income, pass by reference
void print_result(OrderMap order_map, Pricevector vect, char side, double &money, int &sum, int target_size, int timestamp)
{
  //If value still below SIZE after addition/reduction
  if (money == 0.0 && sum < target_size)
  {
    //Nothing to print
  }
  else if (money != 0.0 && sum < target_size) //If value reduced to below SIZE
  {
    money = 0.0;
    cout << timestamp << " " << side << " NA" << endl;
  }
  //If value above size and new prices have been added
  else if (sum >= target_size)
  {
    int counter = target_size;
    money = 0.0;
    double temp_price = 0.0;
    int temp_size = 0;
    int temp_multiplier = 0;
    
    while (counter > 0)
    {
      //Count money through multiplying prices in order
      for (Pricevector::iterator it = vect.begin(); it!= vect.end(); ++it)
      {
        temp_price = it->first;
        temp_size = order_map[it->second].size;
        temp_multiplier = (counter >= temp_size ? temp_size : counter);
        money += temp_multiplier*temp_price;
        counter -= temp_multiplier;
      }
    }
    cout << timestamp << " " << side << " " << money << endl;
  }
}

//take input, and SIZE
int main(int argc, char *argv[]) {
  std::istringstream iss( argv[1] );
  int target_size;
  int bid_sum = 0;
  int ask_sum = 0;
  double bid_expense = 0.0;
  double ask_income = 0.0;
  Pricevector bid;//[price][order-id] descending
  Pricevector ask;//[price][order-id] ascending
  OrderMap order_map;// key = order-id, val = <size, side>
  std::vector<std::string> line_vect;
  string line;
  ifstream myfile ("shortinput.in"); //Modify this filename to use different inputs
  if(myfile.is_open())
  {
    while (getline(myfile,line))//there is a nextline:
    {
      //Split string by whitespace, store vector in line_vect
      split(line,' ',line_vect);
      int timestamp = std::stoi(line_vect[0]);
      
      //If Add Order
      if (line_vect[1].compare("A") == 0)
      {
        Order temp_order;
        temp_order.id = std::stoi(line_vect[2]);
        temp_order.side = line_vect[3][0]; //[0] turns single element string into char??
        temp_order.price = std::stod(line_vect[4]);
        temp_order.size = std::stoi(line_vect[5]);
        //int temp_id = std:stoi(line_vect[2]);
        //int temp_size = std::stoi(line_vect[5]);
        //double temp_price = std::stod(line_vect[4]);
        
        //add order to map, key = id[2], val = size[5],side[3]
        order_map[temp_order.id] = temp_order;
        //order_map[temp_id] = Orderpair(temp_size,line_vect[3]);
        
        if (temp_order.side == 'B')
        {
          //insert price[4], id[2] to bid vector
          ordered_insert(bid,temp_order);
          bid_sum += temp_order.size;
          //call print function map, vector, 'B', bid_expense, bid_Sum
          print_result(order_map, bid, 'B', bid_expense, bid_sum, target_size, timestamp);
        }
        else if (temp_order.side == 'S')
        {
          //insert price[4], id[2] to ask vector
          ordered_insert(ask,temp_order);
          ask_sum += temp_order.size;
          //call print function map, vector, 'S', ask_income, ask_sum
          print_result(order_map, ask, 'S', ask_income, ask_sum, target_size, timestamp);
        }
        else
        {
          cout << "Invalid input format for Side in Add" << endl;
        }
      }
      //If Reduce Order
      else if (line_vect[1].compare("R") == 0)
      {
        //check [2], find order id
        int temp_id = std::stoi(line_vect[2]);
        int temp_size = std::stoi(line_vect[3]);
        int current_size = order_map[temp_id].size;
        //If order is a Bid
        if (order_map[temp_id].side == 'B')
        {
          if (temp_size >= current_size)
          {
            order_map[temp_id].size = 0;
            bid_sum -= current_size;
          }
          else if (temp_size < current_size)
          {
            order_map[temp_id].size -= temp_size;
            bid_sum -= temp_size;
          }
          //call print function map, vector, 'B', bid_expense, bid_sum
          print_result(order_map, bid, 'B', bid_expense, bid_sum, target_size, timestamp);
        }
        else if (order_map[temp_id].side == 'S')
        {
          if (temp_size >= current_size)
          {
            order_map[temp_id].size = 0;
            ask_sum -= current_size;
          }
          else if (temp_size < current_size)
          {
            order_map[temp_id].size -= temp_size;
            ask_sum -= temp_size;
          }
          print_result(order_map, ask, 'S', ask_income, ask_sum, target_size, timestamp);
        }
        else
        {
          cout << "Invalid input format for Side in Reduce" << endl;
        }
      }
      else
      {
        cout << "Invalid input format for Add/Reduce" << endl;
      }
      return 0;
    }
  }
  else
  {
    cout << "File not found" << endl;
    return 0;
  }
}