#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

typedef std::pair<double, int> pricepair //<Price, Order-ID>
typedef std::pair<int,std::string> orderpair //<Size, Side>
//typedef std::vector<pricepair> pricevector

typedef struct Order
{
  int id;
  char side;
  double price;
  int size;
} Order;

take input, and SIZE
int main(int argc, char *argv[]) {
  int bid_sum (buy) = 0
  int ask_sum (sell) = 0
  double bid_expense = 0.0
  double ask_income = 0.0
  std::vector<pricepair> bid;//[price][order-id] descending
  std::vector<pricepair> ask;//[price][order-id] ascending
  std::map<std::string,orderpair> order_map;// key = order-id, val = <size, side>
  std::vector<std::string> line_vect;
  string line;
  ifstream myfile ("price.in");
  if(myfile.is_open())
  {
    while (getline(myfile,line))//there is a nextline:
    {
      //Split string by whitespace, store vector in line_vect
      split(line,' ',line_vect);
      int temp_id = std:stoi(line_vect[2]);
      
      //If Add Order
      if (line_vect[1].compare("A") == 0)
      {
        int temp_size = std::stoi(line_vect[5]);
        double temp_price = std::stod(line_vect[4]);
        //add order to map, key = id[2], val = size[5],side[3]
        order_map[temp_id] = orderpair(temp_size,line_vect[3]);
        //if [3] == "B"
        if (line_vect[3].compare("B") == 0)
        {
          //insert price[4], id[2] to bid vector
          ordered_insert(bid,"B",pricepair(temp_price,temp_id))
          bid_sum += temp_size
          //call print function map, vector, "B", bid_expense, bid_Sum
          print_result(order_map, bid, "B", bid_expense, bid_sum);
        }
        else if (line_vect[3].compare("S") == 0)
        {
          //insert price[4], id[2] to ask vector
          ordered_insert(ask,"S",pricepair(temp_price,temp_id))
          ask_sum += temp_size
          //call print function map, vector, "S", ask_income, ask_sum
          print_result(order_map, ask, "S", ask_income, ask_sum);
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
        int temp_size = std::stoi(line_vect[3]);
        int current_size = order_map[temp_id].first;
        std::string temp_side = order_map[temp_id].second;
        //If order is a Bid
        if (temp_side.compare("B") == 0)
        {
          if (temp_size >= current_size)
          {
            order_map[temp_id].first = 0;
            bid_sum -= current_size;
          }
          else if (temp_size < current_size)
          {
            order_map[temp_id].first -= temp_size;
            bid_sum -= temp_size;
          }
          //call print function map, vector, "B", bid_expense, bid_sum
          print_result(order_map, bid, "B", bid_expense, bid_sum);
        }
        else if (temp_side.compare("S") == 0)
        {
          if (temp_size >= current_size)
          {
            order_map[temp_id].first = 0;
            ask_sum -= current_size;
          }
          else if (temp_size < current_size)
          {
            order_map[temp_id].first -= temp_size;
            ask_sum -= temp_size;
          }
          print_result(order_map, ask, "S", ask_income, ask_sum);
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
      return 0
    }
  }
  else
  {
    cout << "File not found" << endl;
    return 0
  }
}

//Method to split line by delimiter and replace input vector with new vector
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    elems.clear()
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
  
void ordered_insert(std::vector<pricepair> &vect, string side, pricepair value):
  if side == "B"
    ints::iterator it = std::lower_bound( cont.begin(), cont.end(), value.first, std::greater<int>() ); // find proper position in descending order
    cont.insert( it, value ); // insert before iterator it
  else if side == "S"
    ?
  else
    throw error
  
  //Function to print bid_expense, ask_income, pass by reference
take map, vector, side
void print_result(std::map order_map, std::vector<pricepair> vect, std::string side, double money, int sum)
{
  //If value still below SIZE after addition/reduction
  if (money == 0.0 && sum < SIZE)
    no print
  else if money != 0.0 && sum < SIZE //If value reduced to below SIZE
    print NA
    money = 0.0
  else if sum >= SIZE
    int counter = SIZE
    money = 0.0
    double temp_price = 0.0
    int temp_size = 0
    int reduction
    //go through vector
    while counter > 0
      if
      for loop through vector
        temp_price = vect val
        temp_sum = map(id)[0]
        reduction = (counter >= temp_sum ? temp_sum, counter) //Determine if size > counter
        money += temp_price*reduction
        counter -= reduction
    print money
}