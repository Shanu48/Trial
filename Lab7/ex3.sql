SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE list_cars_with_high_damage(threshold IN NUMBER) IS
    CURSOR accident_cursor IS
        SELECT p.regno, p.report_no, SUM(p.damage_amount) AS total_damage
        FROM PARTICIPATED p
        GROUP BY p.regno, p.report_no
        HAVING SUM(p.damage_amount) > threshold;

    v_car_regno PARTICIPATED.regno%TYPE;
    v_report_no PARTICIPATED.report_no%TYPE;
    v_total_damage NUMBER(10, 2);

BEGIN
    -- Open the cursor
    OPEN accident_cursor;

    -- Loop through all the rows in the cursor
    LOOP
        FETCH accident_cursor INTO v_car_regno, v_report_no, v_total_damage;

        EXIT WHEN accident_cursor%NOTFOUND;

        -- Display the car's registration number and total damage for that accident
        DBMS_OUTPUT.PUT_LINE('Car Registration Number: ' || v_car_regno);
        DBMS_OUTPUT.PUT_LINE('Accident Report Number: ' || v_report_no);
        DBMS_OUTPUT.PUT_LINE('Cumulative Damage: ' || v_total_damage);
        DBMS_OUTPUT.PUT_LINE('-------------------------------');
    END LOOP;

    -- Close the cursor
    CLOSE accident_cursor;

END;
/
