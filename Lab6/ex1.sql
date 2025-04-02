SET SERVEROUTPUT ON;

CREATE OR REPLACE FUNCTION total_accidents_in_year(p_year IN INT) 
RETURN INT IS
    v_accidents_count INT;
BEGIN
    SELECT COUNT(*) INTO v_accidents_count
    FROM ACCIDENT
    WHERE EXTRACT(YEAR FROM accd_date) = p_year;
    
    RETURN v_accidents_count;
END;
/
