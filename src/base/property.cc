#include <sstream>
#include "property.hh"
#include "operation.hh"


std::istream& operator >>(std::istream& str, PF::PropertyBase& p)
{
  p.from_stream(str);
  return str;
}
std::ostream& operator <<(std::ostream& str, PF::PropertyBase& p)
{
  p.to_stream(str);
  return str;
}


PF::PropertyBase::PropertyBase(std::string n, OpParBase* par): name(n) 
{
  par->add_property(this);
}


PF::PropertyBase::PropertyBase(std::string n, OpParBase* par, 
			       int val, std::string strval, std::string valname): name(n) 
{
  par->add_property(this);
  add_enum_value( val, strval, valname );
  enum_value.first = val;
  enum_value.second.first = strval;
  enum_value.second.second = valname;
}


void PF::PropertyBase::set_str(const std::string& val)
{
#ifndef NDEBUG
  std::cout<<"PF::PropertyBase::set_str(): setting property \""<<name<<"\" to value \""<<val<<"\""<<std::endl;
#endif
  std::istringstream str(val);
  from_stream(str);
}

std::string PF::PropertyBase::get_str()
{
  std::ostringstream str;
  to_stream(str);
  return str.str();
}


void PF::PropertyBase::from_stream(std::istream& str)
{
#ifndef NDEBUG
  std::cout<<"PF::PropertyBase::from_stream(): is_enum()="<<is_enum()<<std::endl;
#endif
  if( !is_enum() ) return;
  std::string s;
  str>>s;
  std::map< int, std::pair<std::string,std::string> >::iterator iter;
  for( iter = enum_values.begin(); iter != enum_values.end(); iter++ ) {
    if( (*iter).second.first != s ) 
      continue;
    enum_value = (*iter);
    break;
  }
}


void PF::PropertyBase::to_stream(std::ostream& str)
{
  if( !is_enum() ) return;
  if( enum_value.second.first.empty() ) return;
  str<<enum_value.second.first;
}

 

void PF::PropertyBase::set_gobject(gpointer object)
{
  if( !is_enum() ) return;
  if( enum_value.second.first.empty() ) return;
  g_object_set( object, get_name().c_str(), enum_value.first, NULL );
}



template<>
void PF::set_gobject_property<std::string>(gpointer object, const std::string name, const std::string& value)
{
  g_object_set( object, name.c_str(), value.c_str(), NULL );
}



bool PF::PropertyBase::import(PF::PropertyBase* pin)
{
  if( !pin ) 
    return false;

  if( is_enum() ) {
    std::pair< int, std::pair<std::string,std::string> > val = pin->get_enum_value();
    std::map< int, std::pair<std::string,std::string> >::iterator mi = 
      enum_values.find( val.first );
    if( mi == enum_values.end() ) 
      return false;
    std::pair< int, std::pair<std::string,std::string> > val2 = *mi;
    if( (mi->second.first == val2.second.first) &&
	(mi->second.second == val2.second.second) ) {
      enum_value = val;
      return true;
    } else {
      return false;
    }
  } else {
    set_str( pin->get_str() );
    return true;
  }
}
