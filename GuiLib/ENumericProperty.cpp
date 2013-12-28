// ENumericProperty.cpp: implementation of the ENumericProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EPropWndEdit.h"
#include "ENumericProperty.h"
#include "Util.h"

#include <iostream>     
#include <sstream>     
#include <iomanip>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ENumericProperty::ENumericProperty( IPropertyHost* pHost , const CString& sName , void* pProperty , const CString& sUnit , bool bEnabled , double dMinAllowed , double dMaxAllowed ) : EIconTextButtonProperty( pHost , sName , pProperty , bEnabled )
{
	m_sFixedUnit            = sUnit;
	m_dMax                  = dMaxAllowed;
	m_dMin                  = dMinAllowed;
	m_pUnitSymbol           = &m_sFixedUnit;
	m_dScale                = 1.0;
	m_pScaleMul             = NULL;
	m_pScaleDiv             = NULL;
	m_pUnitScaleHost        = NULL;
	m_nFormatFlags          = EIconTextButtonProperty::DEFAULT;
	m_nNumDecimals          = -1;
	m_nNumSignificantDigits = -1;
}

ENumericProperty::~ENumericProperty()
{
}

bool ENumericProperty::GetSortValueDouble( double& dValue )
{
	dValue = GetNumericValue();
	return true;
}

bool ENumericProperty::HasEdit()
{
	return m_pProperty != NULL;
}

ENumericProperty* ENumericProperty::AddSpecialCaseText( double dSpecialCaseValue , CString sText )
{
	SpecialCase sc(dSpecialCaseValue,sText);
	m_vSpecialCases.push_back(sc);
	return this;
}

CString ENumericProperty::GetSpecialCaseText( double dValue )
{
	CString s;

	int n = m_vSpecialCases.size();

	for( int c=0 ; c<n ; c++ )
	{
		if( dValue == m_vSpecialCases[c].m_dValue )
		{
			s = m_vSpecialCases[c].m_sText;
			break;
		}
	}

	return s;
}

ENumericProperty* ENumericProperty::SetScale( double dScale )
{
	m_dScale = dScale;
	return this;
}

ENumericProperty* ENumericProperty::SetScale( const double* pMultiply , const double* pDivide )
{
	m_pScaleMul = pMultiply;
	m_pScaleDiv = pDivide;
	return this;
}

ENumericProperty* ENumericProperty::SetUnit( const CString* pUnitSymbol )
{
	m_pUnitSymbol = pUnitSymbol;
	return this;
}

ENumericProperty* ENumericProperty::SetUnit( UnitScaleHost* pUnitScaleHost )
{
	m_pUnitScaleHost = pUnitScaleHost;
	return this;
}

double ENumericProperty::GetScale()
{
	double dScale = m_dScale;

	if( m_pUnitScaleHost )
	{
		CString s;
		dScale = m_pUnitScaleHost->GetUnitScale(s);
	}
	else
	{
		if( m_pScaleMul != NULL )
		{
			dScale *= (*m_pScaleMul);
		}

		if( m_pScaleDiv != NULL )
		{
			dScale /= (*m_pScaleDiv);
		}
	}

	return dScale;
}

ENumericProperty* ENumericProperty::SetStyle( UINT nFormatFlags , int nNumDecimals )
{
	m_nFormatFlags = nFormatFlags;
	m_nNumDecimals = nNumDecimals;
	return this;
}

ENumericProperty* ENumericProperty::SetNumDecimals( int nNumDecimals )
{
	m_nNumDecimals = nNumDecimals;
	return this;
}

ENumericProperty* ENumericProperty::SetNumSignificantDigits( int nNumSignificantDigits )
{
	m_nNumSignificantDigits = nNumSignificantDigits;
	return this;
}

CString ENumericProperty::GetTextValue()
{
	//
	// Get the numeric value from a type specific sub class (int, float..)
	//

	double dVarValue = (m_pProperty==NULL) ? 1.0 : GetNumericValue();

	//
	// Test if this value has some specific label
	//

	CString s = GetSpecialCaseText(dVarValue);

	//
	// If no specific label, format the numeric value
	//

	if( s.GetLength()==0 )
	{
		double dGuiValue = dVarValue * GetScale();		//scale to gui

		CString sUnit;
		CString sFormat;

		if( ( m_nFormatFlags&EIconTextButtonProperty::SCIENTIFIC) != 0 )
		{
			if( m_nNumDecimals!=-1 )
			{
				sFormat.Format( _T("%%1.%ie"), m_nNumDecimals );
			}
			else
			{
				sFormat.Format( _T("%e") );
			}

			s.Format( sFormat , dGuiValue );
		}
		else
		{
			if( m_nNumDecimals!=-1 )
			{
				sFormat.Format( _T("%%.%ilf") , m_nNumDecimals );
				s.Format( sFormat , dGuiValue );
			}
			else
			{
				//
				// the numeric value should be truncated to only show
				// significan number of digits (not number of decimals!)
				// this is due to the fact that the numeric may not have
				// been a double originally.
				//

				s.Format( _T("%lf"),dGuiValue);		//default if something goes wrong in the "significant" calcultion..

				int nNumSignificantDigits = GetNumSignificantDigits();	//get from type specific subclass

				if( m_nNumSignificantDigits >= 0 )	//or if a custom number of digits has been required, use it
				{
					nNumSignificantDigits = m_nNumSignificantDigits;
				}

				CString sFormat;
				sFormat.Format( _T("%%.%dlf") , nNumSignificantDigits );	//formath the value with this many digits

				CString sSuper;
				sSuper.Format( sFormat , dGuiValue );	//format with enough decimals..

				char cComma = GetCommaCharacter();

				int n = sSuper.GetLength();
				int nCountSignificant = 0;				//count significant digits along string

				bool bFoundComma = false;				//if comma has been found along string

				for( int i=0 ; i<n ; i++ )
				{
					if( '0'<=sSuper[i] && sSuper[i]<='9' )	//a signigicant digit?
					{
						nCountSignificant++;

						if( bFoundComma )
						{
							if( nCountSignificant >= nNumSignificantDigits )	//have we reached the number of digits wanted?
							{
								s = sSuper.Left(i+1);
								break;
							}
						}
					}
					
					if( sSuper[i]==cComma )
					{
						bFoundComma = true;

						if( nCountSignificant >= nNumSignificantDigits )
						{
							s = sSuper.Left(i);
							break;
						}
					}
				}

				//
				// trim away trailing zeros and comma (is any..)
				//

				bool bHasComma = (s.Find(cComma)>0);
				
				if( bHasComma )
				{
					s.TrimRight('0');
					s.TrimRight(cComma);
				}
			}

			
			if( (m_nFormatFlags&EIconTextButtonProperty::THOUSANDS) != 0 )
			{
				s = FormatThousands(s);
			}
		}
		
		if( m_pUnitScaleHost )
		{
			m_pUnitScaleHost->GetUnitScale(sUnit);
		}
		else
		{
			if( m_pUnitSymbol != NULL )
			{
				sUnit = *m_pUnitSymbol;
			}
		}

		if( sUnit.GetLength() > 0 )
		{
			s += " ";
			s += sUnit;
		}
	}

	return s;
}


bool ENumericProperty::GetUserValue( double& dGuiValue )
{
	double d1;
	double d2;

	bool bValid = true;

	if( m_pEdit != NULL )
	{
		CString s;
		m_pEdit->GetWindowText(s);

		//
		//
		//

		char cThousand = GetThousandSeparator();
		char cComma    = (cThousand==',')?('.'):(',');

		//
		// remove unit from string
		//

//		s.TrimRight(' ');
//
//		int n = m_sFixedUnit.GetLength();
//
//		if( n>0 )
//		{
//			CString t = s.Right(n);	//unit from user
//
//			if( m_sFixedUnit.CompareNoCase(t)==0 )	//unit from user is same as fixed unit
//			{
//				s = s.Left( s.GetLength()-n );		//remove unit from string
//			}
//		}

		//
		//	remove spaces
		//

		s.Remove(' ');

		//
		// if thousand seperator is enabled, remove it from the string
		//

		if( (m_nFormatFlags & EIconTextButtonProperty::THOUSANDS) != 0 )
		{
			s.Remove(cThousand);
		}

		//
		// check if thousand separator is used as comma (locale problems maybe?)
		//

//		CString sValidChars = "0123456789-+*/x";
//		sValidChars += cComma;
//
//		n = s.GetLength();
//
//		for( int i=0 ; (i<n) && bValid ; i++ )
//		{
//			char c = s[i];
//			int v = sValidChars.Find(c);
//
//			bValid &= (v>=0);
//
//		}

		//
		//
		//

		if( bValid )
		{
			int nHex = 0;

			if( _stscanf(s,_T("0x%x"),&nHex)==1 )
			{
				dGuiValue = nHex;
			}
			else if( _stscanf(s,_T("%lf/%lf"),&d1,&d2)==2 )
			{
				dGuiValue = d1/d2;
			}
			else if( _stscanf(s, _T("%lf*%lf"),&d1,&d2)==2 )
			{
				dGuiValue = d1*d2;
			}
			else if( _stscanf(s, _T("%lf+%lf"),&d1,&d2)==2 )
			{
				dGuiValue = d1+d2;
			}
			else if( _stscanf(s, _T("%lf-%lf"),&d1,&d2)==2 )
			{
				dGuiValue = d1-d2;
			}
			else if( _stscanf(s, _T("%lf"),&d1)==1 )			//normal situation, entering a new number
			{
				dGuiValue = d1;
			}
			else
			{
				double dOldGuiValue = GetNumericValue() * GetScale();
				
				if( _stscanf(s, _T("/%lf"),&d1)==1 )
				{
					dGuiValue = dOldGuiValue/d1;
				}
				else if( _stscanf(s, _T("*%lf"),&d1)==1 )
				{
					dGuiValue = dOldGuiValue*d1;
				}
				else if( _stscanf(s, _T("+%lf"),&d1)==1 )
				{
					dGuiValue = dOldGuiValue+d1;
				}
				else if( _stscanf(s, _T("-%lf"),&d1)==1 )
				{
					dGuiValue = dOldGuiValue-d1;
				}
				else
				{
					bValid = false;
				}
			}
		}
	}

	return bValid;
}

bool ENumericProperty::CanApply()
{
	TR( _T("ENumericProperty::CanApply()"), GetName());

	double d;
	bool bValidInput = GetUserValue(d);

	if( bValidInput==false )
	{
		ASSERT(m_pEdit!=NULL);
		m_pEdit->SetSel(0,-1);
	}

	return bValidInput;
}

bool ENumericProperty::OnPropApply()
{
	TR( _T("ENumericProperty::OnPropApply()"), GetName());

	ASSERT( m_bSelected == true );
	ASSERT( m_pEdit     != NULL );
	ASSERT( IsEnabled() == true );

	bool bPropertyChanged = false;

	CString s;
	m_pEdit->GetWindowText(s);

	if( s!=m_sInitialValueText )
	{
		double dGuiValue;
		bool bValidInput = GetUserValue(dGuiValue);

		if( bValidInput )
		{
			double dVarValue = dGuiValue / GetScale();		//gui -> var

			if( (m_dMin==0 && m_dMax==0) || (m_dMin<=dVarValue && dVarValue<=m_dMax) )	//within range?
			{
				bPropertyChanged = SetNumeric(dVarValue);
			}
		}
	}

	return bPropertyChanged;
}

ENumericProperty* ENumericProperty::SetRange( double dMin , double dMax )
{
	m_dMin = min( dMin , dMax );
	m_dMax = max( dMin , dMax );
	return this;
}
