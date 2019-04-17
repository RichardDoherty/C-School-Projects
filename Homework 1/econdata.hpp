
struct county {
  std::string name;
  float unemployed_2007;
  float unemployed_2015;
  int med_income;
};

struct state {
  std::string name;
  float unemployed_2007;
  float unemployed_2015;
  int med_income;
  struct county* counties;
  int n_counties;
};

struct state* allocate_states(int);

void read_state_data(struct state*, int, std::ifstream&);

struct county* allocate_counties(int);

void read_county_data(struct county*, int, std::ifstream&);

void free_state_data(struct state*, int);

bool cmp_med_income(const struct state&, const struct state&);
bool cmp_unemployment_2015(const struct state&, const struct state&);
bool cmp_unemployment_2007(const struct state&, const struct state&);


bool cmp_med_income_county(const struct county&, const struct county&);
bool cmp_unemployment_2015_county(const struct county&, const struct county&);
bool cmp_unemployment_2007_county(const struct county&, const struct county&);

void county_info(std::string &, std::string &, std::string &);

void state_info(std::string &, std::string &, std::string &);

void county_output(std::string, std::string, std::string, int, struct state*);

void state_output(std::string, std::string, std::string, struct state*, int);
