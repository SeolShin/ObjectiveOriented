#ifndef	INCLUDE_CALC_H
#define	INCLUDE_CALC_H

class CCalc{
	int m_nValue,m_nMax;
	BOOL m_bEnd;

public:
	void init(int max){	//������
		m_nValue=0;m_nMax=max;
		m_bEnd=FALSE;
	}

	void execute(){	//�又��
		while(m_nValue<m_nMax){
			m_nValue++;

			//���ׂ̂����鏈��
			CString s;s.Format("%d",m_nValue);
		}
	}

	int getValue() const{	//�l�擾
		return m_nValue;
	}
	
	void setEnd(){
		m_bEnd=TRUE;
	}

	BOOL isEnd() const{	//�I���������ǂ���
		return m_bEnd;
	}
};

#endif	//INCLUDE_CALC_H