SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE total_damage_for_driver_in_year(
    p_driver_id IN VARCHAR2,
    p_year IN INT
) IS
    v_total_damage INT;
BEGIN
    SELECT SUM(damage_amount) INTO v_total_damage
    FROM PARTICIPATED p
    JOIN ACCIDENT a ON p.report_no = a.report_no
    WHERE p.driver_id = p_driver_id
    AND EXTRACT(YEAR FROM a.accd_date) = p_year;

    DBMS_OUTPUT.PUT_LINE('Total Damage for Driver ' || p_driver_id || ' in Year ' || p_year || ' is ' || v_total_damage);
END;
/
