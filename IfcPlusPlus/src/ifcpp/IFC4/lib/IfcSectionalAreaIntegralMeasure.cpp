/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcSectionalAreaIntegralMeasure.h"

// TYPE IfcSectionalAreaIntegralMeasure = REAL;
IfcSectionalAreaIntegralMeasure::IfcSectionalAreaIntegralMeasure() {}
IfcSectionalAreaIntegralMeasure::IfcSectionalAreaIntegralMeasure( double value ) { m_value = value; }
IfcSectionalAreaIntegralMeasure::~IfcSectionalAreaIntegralMeasure() {}
shared_ptr<IfcPPObject> IfcSectionalAreaIntegralMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSectionalAreaIntegralMeasure> copy_self( new IfcSectionalAreaIntegralMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcSectionalAreaIntegralMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSECTIONALAREAINTEGRALMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSectionalAreaIntegralMeasure> IfcSectionalAreaIntegralMeasure::createObjectFromSTEP( const std::wstring& arg, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSectionalAreaIntegralMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSectionalAreaIntegralMeasure>(); }
	shared_ptr<IfcSectionalAreaIntegralMeasure> type_object( new IfcSectionalAreaIntegralMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}