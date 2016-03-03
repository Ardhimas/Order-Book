#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef struct Order
{
  std::string id;
  char side;
  double price;
  int size;
} Order;

typedef std::pair<double, std::string> Pricepair; //<Price, Order-ID>
typedef std::pair<int,std::string> Orderpair; //<Size, Side>
typedef std::vector<Pricepair> Pricevector;
typedef std::map<std::string,Order> OrderMap;


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
  //d is new price, true when vect val > new price
  auto descending_cmp =  [](Pricepair const & x, double d) -> bool
    { return x.first >= d; };//> OR >=? TODO
  auto ascending_cmp =  [](Pricepair const & x, double d) -> bool
    { return x.first < d; };//> OR >=? TODO
  if (order.side == 'B')
  {
    //Insert element to maintain descending order of bid list
    auto it = std::lower_bound( vect.begin(), vect.end(), order.price, descending_cmp); // find proper position in descending order
    vect.insert( it, Pricepair(order.price,order.id)); // insert before iterator it
  }
  else if (order.side == 'S')
  {
    //Insert element to maintain ascending order of ask list
    auto it = std::lower_bound( vect.begin(), vect.end(), order.price, ascending_cmp);
    vect.insert( it, Pricepair(order.price,order.id)); // insert before iterator it
  }
  else
    cout << "Error invalid input side in ordered_insert" << endl;
}
  
  //Function to print bid_expense, ask_income, pass by reference
void print_result(OrderMap order_map, Pricevector vect, char side, double &money, int &sum, int target_size, int timestamp)
{
  //cout << "Going into print_result" << endl;
  //If sum still below target_size after addition/reduction
  if (money == 0.0 && sum < target_size)
  {
    //Nothing to print, uncomment to debug
    //cout << "Value still below, sum for " << side << " = " << sum << endl;
  }
  //If sum reduced to below target_size but previously above or equal
  else if (money != 0.0 && sum < target_size) 
  {
    money = 0.0;
    cout << timestamp << " " << side << " NA" << endl;
  }
  //If sum above or equal to target_size after addition/reduction
  else if (sum >= target_size)
  {
    int counter = target_size;
    money = 0.0;
    double temp_price = 0.0;
    int temp_size = 0;
    int temp_multiplier = 0;
    // Uncomment to debug vector values
    // for (int i = 0; i < vect.size(); i++)
    //   cout << vect[i].first << endl;
    
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
    cout << timestamp << " " << side << " " << std::setprecision(2) << std::fixed << money << endl;
  }
  //cout << "leaving print_Result" << endl;
}

//Main takes target_size as only command line input
int main(int argc, char *argv[]) {
  std::istringstream iss( argv[1] );
  int target_size;
  iss >> target_size;
  //cout << "target_size=" << target_size << endl;
  int bid_sum = 0;
  int ask_sum = 0;
  double bid_expense = 0.00;
  double ask_income = 0.00;
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
      // for (int i = 0; i < line_vect.size(); i++)
      //   cout << line_vect[i] << endl;
      int timestamp = std::stoi(line_vect[0]);
      
      //If Add Order
      if (line_vect[1].compare("A") == 0)
      {
        //Construct Order and add to map using order id as key
        Order temp_order;
        temp_order.id = line_vect[2];
        temp_order.side = line_vect[3][0]; //[0] turns single element string into char
        temp_order.price = std::stod(line_vect[4]);
        temp_order.size = std::stoi(line_vect[5]);
        order_map[temp_order.id] = temp_order;
        
        if (temp_order.side == 'B')
        {
          ordered_insert(bid,temp_order);
          bid_sum += temp_order.size;
          //call print function map, vector, 'B', bid_expense, bid_Sum
          print_result(order_map, bid, 'S', bid_expense, bid_sum, target_size, timestamp);
        }
        else if (temp_order.side == 'S')
        {
          //insert price[4], id[2] to ask vector
          ordered_insert(ask,temp_order);
          ask_sum += temp_order.size;
          //call print function map, vector, 'S', ask_income, ask_sum
          print_result(order_map, ask, 'B', ask_income, ask_sum, target_size, timestamp);
        }
        else
        {
          cout << "Invalid input format for Side in Add" << endl;
        }
      }
      //If Reduce Order
      else if (line_vect[1].compare("R") == 0)
      {
        string temp_id = line_vect[2];
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
          print_result(order_map, bid, 'S', bid_expense, bid_sum, target_size, timestamp);
        }
        //If order is an Ask
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
          print_result(order_map, ask, 'B', ask_income, ask_sum, target_size, timestamp);
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
    }
    //return 0;
  }
  else
  {
    cout << "File not found" << endl;
    //return 0;
  }
  return 0;
}