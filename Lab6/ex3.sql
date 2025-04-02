SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE accident_info_by_location(p_location IN VARCHAR2) IS
BEGIN
    FOR r IN (
        SELECT * FROM ACCIDENT
        WHERE location = p_location
    ) LOOP
        DBMS_OUTPUT.PUT_LINE('Report Number: ' || r.report_no || ', Date: ' || r.accd_date || ', Location: ' || r.location);
    END LOOP;
END;
/
