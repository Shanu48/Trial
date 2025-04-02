SET SERVEROUTPUT ON;

CREATE OR REPLACE FUNCTION total_accidents_for_car(p_regno IN VARCHAR2)
RETURN INT IS
    v_accident_count INT;
BEGIN
    SELECT COUNT(*) INTO v_accident_count
    FROM PARTICIPATED p
    WHERE p.regno = p_regno;

    RETURN v_accident_count;
END;
/
