---
--- CLEAR MUSIC DATA
-- delete from world1.ms_time_code;
-- delete from world1.ms_sequence;
-- delete from world1.ms_pattern;
---


CREATE DATABASE algebrakart;

-- DROP SCHEMA world1;

CREATE SCHEMA world1 AUTHORIZATION postgres;


-- world1.ms_seq_id definition

-- DROP SEQUENCE world1.ms_seq_id;

CREATE SEQUENCE world1.ms_seq_id
    INCREMENT BY 1
    MINVALUE 1
    MAXVALUE 9223372036854775807
    START 1
	CACHE 1
	NO CYCLE;


-- world1.ms_pattern definition

-- Drop table

-- DROP TABLE world1.ms_pattern;

CREATE TABLE world1.ms_pattern (
                                   seq_id int8 NOT NULL,
                                   time_code float8 NOT NULL,
                                   channel int4 NOT NULL,
                                   sample_idx int4 NOT NULL,
                                   CONSTRAINT ms_pattern_pk PRIMARY KEY (seq_id, time_code, channel)
);

-- Permissions

ALTER TABLE world1.ms_pattern OWNER TO postgres;
GRANT ALL ON TABLE world1.ms_pattern TO postgres;


-- world1.ms_sequence definition

-- Drop table

-- DROP TABLE world1.ms_sequence;

CREATE TABLE world1.ms_sequence (
                                    seq_id int8 NOT NULL,
                                    seq_name varchar NOT NULL,
                                    created timestamp NOT NULL,
                                    CONSTRAINT ms_sequence_pk PRIMARY KEY (seq_id)
);

-- Permissions

ALTER TABLE world1.ms_sequence OWNER TO postgres;
GRANT ALL ON TABLE world1.ms_sequence TO postgres;


-- world1.ms_time_code definition

-- Drop table

-- DROP TABLE world1.ms_time_code;

CREATE TABLE world1.ms_time_code (
                                     time_code float8 NOT NULL,
                                     beat float8 NOT NULL,
                                     rec_date timestamp NOT NULL,
                                     CONSTRAINT ms_time_code_pk PRIMARY KEY (rec_date, time_code)
);

-- Permissions

ALTER TABLE world1.ms_time_code OWNER TO postgres;
GRANT ALL ON TABLE world1.ms_time_code TO postgres;




-- Permissions

GRANT ALL ON SCHEMA world1 TO postgres;
