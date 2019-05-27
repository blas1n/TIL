#pragma once

#ifndef __STRING_FIELD__
#define __STRING_FIELD__

namespace StringField {
	using cstring = const char*;

	static cstring INTRO = "자리 배치 프로그램";
	static cstring END = "프로그램 종료";
	static cstring SELECT = "1 : 배치, 2 : 출력, 3 : 열거하여 보기, 4 : 학생 추가, 5 : 학생 삭제, 0 : 종료. ";
	static cstring ADD_STUDENT = "추가할 학생의 이름을 입력하세요. ";
	static cstring DEL_STUDENT = "삭제할 학생의 이름을 입력하세요. ";
	static cstring START_ARRANGE = "자리를 배치합니다";
	static cstring END_ARRANGE = "배치가 완료되었습니다";
	static cstring FILE_ERROR = "파일을 열 수 없습니다";
	static cstring NULL_POS = "없는 자리";
}

#endif