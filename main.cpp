#include <vector>
#include <set>
#include <iterator>

using namespace std;

struct FloorButtons {
  bool up;
  bool down;

  FloorButtons() : up(false), down(false) {}
};

struct Lift {
  int cur_floor;
  bool direction;
  vector<bool> buttons;
  multiset<int> people;

  Lift() : direction(true), cur_floor(0) {}
};

vector<int> the_lift(vector<vector<int>> queues, int capacity) {
  vector<int> floors_order;
  vector<FloorButtons> floors_state;
  int floors_number = queues.size();
  floors_state.reserve(floors_number);
  Lift lift;
  lift.buttons.assign(floors_number, false);
  
  for (int i = 0; i < floors_number; ++i) {
    floors_state.emplace_back();
    if (!queues.at(i).empty()) {
      for (int j = 0; j < queues.at(i).size(); ++j) {
        if (floors_state.at(i).up && floors_state.at(i).down) break;
        if (queues.at(i).at(j) > i) {
          floors_state.at(i).up = true;
        } else {
          floors_state.at(i).down = true;
        }
      }
    }
  }

  while (true) {
    floors_order.emplace_back(lift.cur_floor);
    lift.buttons.at(lift.cur_floor) = false;
    bool people_same_direction = false;

    if (lift.direction) {
      floors_state.at(lift.cur_floor).up = false;
    } else {
      floors_state.at(lift.cur_floor).down = false;
    }

    auto it = lift.people.equal_range(lift.cur_floor);
    if (distance(it.first, it.second) != 0) {
      lift.people.erase(it.first, it.second);
    }

    for (int i = 0; i < queues.at(lift.cur_floor).size(); ++i) {
      if (lift.people.size() == capacity) {
        for (int i = 0; i < queues.at(lift.cur_floor).size(); ++i) {
          if (floors_state.at(lift.cur_floor).up && floors_state.at(lift.cur_floor).down) break;
          if (queues.at(lift.cur_floor).at(i) > lift.cur_floor) {
            floors_state.at(lift.cur_floor).up = true;
          } else {
            floors_state.at(lift.cur_floor).down = true;
          }
        }
        break;
      }
      if ((lift.direction && (queues.at(lift.cur_floor).at(i) > lift.cur_floor)) ||
      (!lift.direction && (queues.at(lift.cur_floor).at(i) < lift.cur_floor))) {
        people_same_direction = true;
        lift.people.insert(queues.at(lift.cur_floor).at(i));
        lift.buttons.at(queues.at(lift.cur_floor).at(i)) = true;
        queues.at(lift.cur_floor).erase(queues.at(lift.cur_floor).begin() + i);
        --i;
      }
    }

    int lift_button = -1;
    bool lift_button_found = false;

    int floor_button = -1;
    bool floor_button_found = false;

    if (lift.direction) {
      for (int i = lift.cur_floor+1; i < floors_number; ++i) {
        if (!lift_button_found && lift.buttons.at(i)) {
          lift_button = i;
          lift_button_found = true;
        }
        if (!floor_button_found && floors_state.at(i).up) {
          floor_button = i;
          floor_button_found = true;
        }
      }
    } else {
      for (int i = lift.cur_floor-1; i > -1; --i) {
        if (!lift_button_found && lift.buttons.at(i)) {
          lift_button = i;
          lift_button_found = true;
        }
        if (!floor_button_found && floors_state.at(i).down) {
          floor_button = i;
          floor_button_found = true;
        }
      }
    }

    if (lift_button_found || floor_button_found) {
      if (lift.direction) {
        lift.cur_floor = min(lift_button, floor_button, [](const int &a, const int &b) {
          if (a == -1) return false;
          if (b == -1) return true;
          return (a < b) ? true : false;
        });
      } else {
        lift.cur_floor = max(lift_button, floor_button, [](const int &a, const int &b) {
          if (a == -1) return true;
          if (b == -1) return false;
          return (a < b) ? true : false;
        });
      }
    } else {
      if (lift.direction) {
        for (int i = lift.cur_floor+1; i < floors_number; ++i) {
          if(floors_state.at(i).down) {
            floor_button = i;
            floor_button_found = true;
          }
        }
      } else {
        for (int i = lift.cur_floor-1; i > -1; --i) {
          if(floors_state.at(i).up) {
            floor_button = i;
            floor_button_found = true;
          }  
        }
      }
      if (floor_button_found) {
        lift.cur_floor = floor_button;
      } else {
        if (lift.direction) {
          floors_state.at(lift.cur_floor).down = false;
        } else {
          floors_state.at(lift.cur_floor).up = false;
        }

        for (int i = 0; i < queues.at(lift.cur_floor).size(); ++i) {
          if (lift.people.size() == capacity) {
            for (int i = 0; i < queues.at(lift.cur_floor).size(); ++i) {
              if (floors_state.at(lift.cur_floor).up && floors_state.at(lift.cur_floor).down) break;
              if (queues.at(lift.cur_floor).at(i) > lift.cur_floor) {
                floors_state.at(lift.cur_floor).up = true;
              } else {
                floors_state.at(lift.cur_floor).down = true;
              }
            }
            break;
          }
          if ((lift.direction && (queues.at(lift.cur_floor).at(i) < lift.cur_floor)) ||
          (!lift.direction && (queues.at(lift.cur_floor).at(i) > lift.cur_floor))) {
            lift.people.insert(queues.at(lift.cur_floor).at(i));
            lift.buttons.at(queues.at(lift.cur_floor).at(i)) = true;
            queues.at(lift.cur_floor).erase(queues.at(lift.cur_floor).begin() + i);
            --i;
          }
        }
        if (lift.direction) {
          lift.direction = false;
          for (int i = lift.cur_floor-1; i > -1; --i) {
            if (!lift_button_found && lift.buttons.at(i)) {
              lift_button = i;
              lift_button_found = true;
            }
            if (!floor_button_found && floors_state.at(i).down) {
              floor_button = i;
              floor_button_found = true;
            }
          }
        } else {
          lift.direction = true;
          for (int i = lift.cur_floor+1; i < floors_number; ++i) {
            if (!lift_button_found && lift.buttons.at(i)) {
              lift_button = i;
              lift_button_found = true;
            }
            if (!floor_button_found && floors_state.at(i).up) {
              floor_button = i;
              floor_button_found = true;
            }
          }
        }
        if (lift_button_found || floor_button_found) {
          if (lift.direction) {
            lift.cur_floor = min(lift_button, floor_button, [](const int &a, const int &b) {
              if (a == -1) return false;
              if (b == -1) return true;
              return (a < b) ? true : false;
            });
          } else {
            lift.cur_floor = max(lift_button, floor_button, [](const int &a, const int &b) {
              if (a == -1) return true;
              if (b == -1) return false;
              return (a < b) ? true : false;
            });
          }
        } else {
          if (lift.direction) {
            for (int i = lift.cur_floor-1; i > -1; --i) {
              if (!floor_button_found && floors_state.at(i).down) {
                floor_button = i;
                floor_button_found = true;
              }  
            }
          } else {
            for (int i = lift.cur_floor+1; i < floors_number; ++i) {
              if (!floor_button_found && floors_state.at(i).up) {
                floor_button = i;
                floor_button_found = true;
              }
            }
          }
          if (floor_button_found) {
            lift.cur_floor = floor_button;
          } else {
            if (lift.direction) {
              for (int i = lift.cur_floor+1; i < floors_number; ++i) {
                if(floors_state.at(i).down) {
                  floor_button = i;
                  floor_button_found = true;
                }  
              }
            } else {
              for (int i = lift.cur_floor-1; i > -1; --i) {
                if (floors_state.at(i).up) {
                  floor_button = i;
                  floor_button_found = true;
                }  
              }
            }
            if (floor_button_found) {
              lift.cur_floor = floor_button;
            } else {
              if (lift.cur_floor != 0) { 
                floors_order.emplace_back(0);
                break;
              } else {
                break;
              }
            }
          }
        }
      }
    }   
  }  
  return floors_order;
}
