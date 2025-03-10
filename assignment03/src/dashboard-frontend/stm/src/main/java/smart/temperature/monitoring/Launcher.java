package smart.temperature.monitoring;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 *  Main Class of the JavaFX Application (Smart Temperature Monitoring)
 * 
 *  @see Application
 */
public class Launcher extends Application {
    public static final String APPLICATION_NAME = "Smart Temperature Monitoring"; /** Application Name */
    public static final String APPLICATION_LAYOUT = "STM.fxml"; /** Application Layout Filename (resources) */
    public static final boolean APPLICATION_RESIZABLE = false; /** Application Resizable Flag */

    @Override
    public void start(final Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource(APPLICATION_LAYOUT)); // Load the FXML file
        GraphicalController controller = new GraphicalController();
        loader.setController(controller);

        final Parent root = loader.load();
        final Scene scene = new Scene(root);

        primaryStage.setScene(scene);
        primaryStage.setTitle(APPLICATION_NAME);
        primaryStage.setResizable(APPLICATION_RESIZABLE);

        primaryStage.setOnCloseRequest((closeRequest) -> { // Close the application when the window is closed
            Platform.exit();
            System.exit(0);
        });

        primaryStage.show(); // Show the application window
    }
}