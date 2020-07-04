# JXCP1 Controll from B&R Automation PLC

## Requirements:

**Software:**
```bash
Automation Studio BR Automation
SMC (JXCP1) Driver for Windows 10
```

Link AS B&R: https://www.br-automation.com/en/downloads/ 

Driver for Windows 10:

Link: https://www.smcworld.com/products/pickup/en-jp/electric_actuator/download.html

Repositary: /JXCP1_BR_SMC/Driver_for_Windows10/

**Hardware:**

**24VDC Fieldbus – JXC*1**

Link: https://www.smcusa.com/products/electric-actuators/controllers-drivers/24vdc-fieldbus/jxc-1~135950

## Project Description:

**Repositary [JXCP1_BR_SMC/Automation_Studio/JXCP_RPOFINET_project]:**

```bash
[ AS Library         ] /Logical/Libraries/rp_funcx/
[ AS Test Real-HW    ] /Logical/Technology/JXCP_Test/
[ AS MappView        ] /Logical/mappView/
```
**AS Project Runtime Version:**

<p align="center">
<img src="https://github.com/rparak/JXCP1_BR_SMC/blob/master/images/Version.PNG" width="382" height="533">
</p>

**Wiring Diagram:**

<p align="center">
<img src="https://github.com/rparak/JXCP1_BR_SMC/blob/master/images/wiring_diagram.PNG" width="680" height="500">
</p>

**Project Structure (Logical View):**

<p align="center">
<img src="https://github.com/rparak/JXCP1_BR_SMC/blob/master/images/LV.PNG" width="382" height="533">
</p>

**MappView:**

IP Address (Simulation/Real HW): 

http://localhost:81/index.html?visuId=jxcp_profinet or http://PLC_ADDRESS:81/index.html?visuId=jxcp_profinet

<p align="center">
<img src="https://github.com/rparak/JXCP1_BR_SMC/blob/master/images/mv.png" width="800" height="450">
</p>

## Contact Info:
Roman.Parak@outlook.com

## License
[MIT](https://choosealicense.com/licenses/mit/)
