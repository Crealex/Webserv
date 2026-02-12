#ifndef PRINT_DEBUG_HPP

# define PRINT_DEBUG_HPP

struct structParse;
struct server;
class Config;

void	printConfigStruct(structParse config);
void 	printConfigClass(Config conf);
void 	printStructV2(server srv);

#endif // DEBUG_HPP
