package control.unit.connections.serial;

import java.util.concurrent.*;

import control.unit.ValueManager;
import jssc.*;

/**
 * A Communication Channel that uses a Serial Port
 * 
 * @author Alessandro Ricci
 * 
 * @see CommunicationChannel CommunicationChannel (Implemented Interface)
 * @see SerialPortEventListener SerialPortEventListener (Implemented Interface)
 * @see https://github.com/aricci303/ Author's GitHub Profile
 */
public class SerialAgent implements CommunicationChannel, SerialPortEventListener {
    private static final int QUEUE_SIZE = 100; /** Queue Size for Storing Messages */

    private static final String TEMPERATURE_TAG = "temperature:"; /** Temperature Tag */
    private static final String APERTURE_TAG = "aperture:"; /** Aperture Tag */
    private static final String MODE_TAG = "mode:"; /** Mode Tag */

    private final SerialPort serialPort; /** Serial Port Object */
    private final BlockingQueue<String> queue; /** Blocking Queue for Storing Messages */
    private StringBuffer currentMessage = new StringBuffer(""); /** Current Message Buffer */

    private final ValueManager valueManager; /** Value Manager Object */

    /**
     * Constructor for SerialChannel
     * 
     * @param port Serial Port Name
     * @param rate Baud Rate
     * 
     * @throws SerialPortException
     */
    public SerialAgent(String port, int rate, ValueManager valueManager) throws SerialPortException {
        this.queue = new ArrayBlockingQueue<String>(QUEUE_SIZE);
        
        this.serialPort = new SerialPort(port);
        this.serialPort.openPort();
        this.serialPort.setParams(rate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        this.serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
        this.serialPort.addEventListener(this);

        this.valueManager = valueManager;
    }

    @Override
    public boolean sendMessage(String msg) {
		char[] array = (msg+"\n").toCharArray();
		byte[] bytes = new byte[array.length];
		for(int i = 0; i < array.length; i++){
			bytes[i] = (byte) array[i];
		}
		try {
			synchronized (serialPort) {
                return serialPort.writeBytes(bytes);
			}
		}
        catch(Exception ex){
			ex.printStackTrace();
            return false;
		}
	}

    @Override
    public String receiveMessage() throws InterruptedException {
        return queue.take();
    }

    @Override
    public boolean isMessageAvailable() {
        return !queue.isEmpty();
    }

    @Override
    public void closeChannel() {
        try {
            if(serialPort != null) {
                serialPort.removeEventListener();
                serialPort.closePort();
            }
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public void serialEvent(SerialPortEvent event) {
        if(event.isRXCHAR()) {
            try {
                String msg = serialPort.readString(event.getEventValue());
                msg = msg.replaceAll("\r", "");

                currentMessage.append(msg);

                boolean goAhead = true;

                while(goAhead) {
                    String msg2 = currentMessage.toString();
                    int index = msg2.indexOf("\n");
                    if(index >= 0) {
                        queue.put(msg2.substring(0, index));
                        currentMessage = new StringBuffer("");
                        if(index + 1 < msg2.length()) {
                            currentMessage.append(msg2.substring(index + 1)); 
                        }
                    }
                    else {
                        goAhead = false;
                    }
                }

            }
            catch(Exception ex) {
                ex.printStackTrace();
                System.out.println("Error in receiving string from COM-port: " + ex);
            }
        }
    }

    @Override
    public boolean isOpened() {
        return serialPort.isOpened();
    }

    @Override
    public SerialPort getSerialPort() {
        return serialPort;
    }

    /**
     * Send current temperature to serial port
     * 
     * @return true if message was sent successfully
     */
    public boolean sendTemperature() {
        double temp = this.valueManager.getCurrentTemperature();

        return this.sendMessage(TEMPERATURE_TAG + temp);
    }

    /**
     * Send current mode to serial port
     * 
     * @return true if message was sent successfully
     */
    public boolean sendMode() {
        String mode = this.valueManager.getMode() == ValueManager.Mode.AUTOMATIC ? "automatic" : "manual";

        return this.sendMessage(MODE_TAG + mode);
    }

    /**
     * Get Mode from serial port
     * 
     * @return if message was received successfully
     */
    public boolean receiveData() {
        if(!this.isMessageAvailable()) {
            return false;
        }

        System.out.println("Data received from Serial Port!");

        try {
            // Get clear message, remove new line and carriage return characters
            String message = this.receiveMessage().replace("\n", "").replace("\r", "");

            System.out.println("Message: " + message);

            if(message.startsWith(MODE_TAG)) { // If the message is a mode message
                String mode = message.substring(MODE_TAG.length()); // Get value and
                this.valueManager.setMode(mode); // Set current mode
            }
            else if(message.startsWith(APERTURE_TAG)) { // If the message is an aperture message
                int aperture = Integer.parseInt(message.substring(APERTURE_TAG.length())); // Get value and
                this.valueManager.setAperture(aperture); // Set current aperture
            }
        }
        catch(InterruptedException e) {
            e.printStackTrace();
        }

        return true;
    }

    /**
     * Get mode value
     * 
     * @return Mode Value
     * @see #mode
     */
    public ValueManager.Mode getMode() {
        return this.valueManager.getMode();
    }

    /**
     * Send current aperture to serial port
     * 
     * @return true if message was sent successfully
     */
    public boolean sendAperture() {
        int aperture = this.valueManager.getCorrespondingAperture();

        return this.sendMessage(APERTURE_TAG + aperture);
    }
}
