// ENumericProperty.h: interface for the ENumericProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMERICPROPERTY_H__B74ECABC_E561_4A5B_A9C2_D59151A7BA9A__INCLUDED_)
#define AFX_ENUMERICPROPERTY_H__B74ECABC_E561_4A5B_A9C2_D59151A7BA9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EIconTextButtonProperty.h"
#include <vector>

class ENumericProperty : public EIconTextButtonProperty
{

protected:

	//
	// someone to ask about unitscale and symbol
	//

	class UnitScaleHost
	{
	public:
		virtual double GetUnitScale( CString& sSymbol )
		{
			return 1.0;
		}
	};

	//
	//
	//

	class SpecialCase
	{
	public:
		double m_dValue;
		CString m_sText;
		SpecialCase( double dValue , CString sText )
		{
			m_dValue = dValue;
			m_sText  = sText;
		}
	};

	std::vector<SpecialCase> m_vSpecialCases;

protected:

	double  m_dScale;		//scale the property by this value before showing on screen (eg for showing value in another unit)
	CString m_sFixedUnit;
	const CString* m_pUnitSymbol;
	const double*  m_pScaleMul;
	const double*  m_pScaleDiv;
	double m_dMin;
	double m_dMax;
	UnitScaleHost* m_pUnitScaleHost;
	UINT m_nFormatFlags;
	int m_nNumDecimals;
	int m_nNumSignificantDigits;

public:

	ENumericProperty( IPropertyHost* pHost , const CString& sName , void* pProperty , const CString& sUnit , bool bEnabled , double dMinAllowed , double dMaxAllowed );
	virtual ~ENumericProperty();

protected:

	CString GetSpecialCaseText( double dValue );
	double GetScale();
	CString GetTextValue();

protected:

	//
	// EProperty
	//
	virtual bool GetSortValueDouble( double& dValue );
	virtual bool OnPropApply();
	virtual bool CanApply();

	//
	// EIconTextButtonProperty
	//
	virtual bool HasEdit();

	//
	// this
	//
	virtual double GetNumericValue() = 0;
	virtual bool SetNumeric( double dVarValue ) = 0;
	virtual int GetNumSignificantDigits() = 0;
	bool GetUserValue( double& dGuiValue );

public:

	//
	// this
	//

	ENumericProperty* AddSpecialCaseText( double dSpecialCaseValue , CString sText );	//If a numeric has this special case value, a text message is written instead of the number itself. For example let -1 be a "N/A" special case if this means, not initialized.
	ENumericProperty* SetScale( double dScale );	//Scale the value to gui by this value. If you member variable is stored in cm, you may want to set the scale to 0.01, and have the unit say "m".
	ENumericProperty* SetScale( const double* pMultiply , const double* pDivide );		//If you have scales depending on some other settings, you can have the numeric property point to where ever your scaling values are stored.
	ENumericProperty* SetUnit( const CString* pUnitSymbol );	//If your unit changes, this points to here to find that unit.
	ENumericProperty* SetUnit( UnitScaleHost* pUnitScaleHost );	//If you unit changes, you can have a unithost tell how to scale, and what unit to use.
	ENumericProperty* SetStyle( UINT nFormatFlags=EIconTextButtonProperty::DEFAULT , int nNumDecimals=-1 );	//Set formatting flags.
	ENumericProperty* SetRange( double dMin , double dMax );	//Only values within this range are allowed, if user enters a value out of this range, the change is cancelled.
	ENumericProperty* SetNumDecimals( int nNumDecimals );	//Number of decimals to put to the in gui shown value.
	ENumericProperty* SetNumSignificantDigits( int nNumSignificantDigits );	//Sets the number of significant digits to present to the user.


};

#endif // !defined(AFX_ENUMERICPROPERTY_H__B74ECABC_E561_4A5B_A9C2_D59151A7BA9A__INCLUDED_)
