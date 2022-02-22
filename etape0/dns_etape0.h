//

typedef struct header_u{
	uint16_t ID;
	uint16_t FLAGS;
	uint16_t QDCOUNT;
	uint16_t ANCOUNT;
	uint16_t NSCOUNT;
	uint16_t ARCOUNT;
}header_t;

typedef struct question_u{
	uint16_t QTYPE;
	uint16_t QCLASS;
}question_t;


