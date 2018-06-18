#ifndef	INCLUDE_CALC_H
#define	INCLUDE_CALC_H

class CCalc{
	int m_nValue,m_nMax;
	BOOL m_bEnd;

public:
	void init(int max){	//‰Šú‰»
		m_nValue=0;m_nMax=max;
		m_bEnd=FALSE;
	}

	void execute(){	//åˆ—
		while(m_nValue<m_nMax){
			m_nValue++;

			//•‰‰×‚Ì‚©‚©‚éˆ—
			CString s;s.Format("%d",m_nValue);
		}
	}

	int getValue() const{	//’læ“¾
		return m_nValue;
	}
	
	void setEnd(){
		m_bEnd=TRUE;
	}

	BOOL isEnd() const{	//I—¹‚µ‚½‚©‚Ç‚¤‚©
		return m_bEnd;
	}
};

#endif	//INCLUDE_CALC_H