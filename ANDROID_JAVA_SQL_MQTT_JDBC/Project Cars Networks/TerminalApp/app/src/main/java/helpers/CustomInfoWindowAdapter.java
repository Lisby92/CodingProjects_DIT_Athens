package helpers;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.Marker;

import gr.di.uoa.R;

public class CustomInfoWindowAdapter implements GoogleMap.InfoWindowAdapter
{

  private final View custWindow;
  private Context custContext;

  //constructor
  public CustomInfoWindowAdapter(Context context)
  {
    custContext = context;
    custWindow = LayoutInflater.from(context).inflate(R.layout.custom_info_window, null);
  }

  //this method sets the text to the view
  private void addWindowText(Marker marker, View view)
  {
    String title = marker.getTitle();
    TextView tvTitle = view.findViewById(R.id.infoTitle);
    tvTitle.setText(title);

    String snippet = marker.getSnippet();
    TextView tvSnippet = view.findViewById(R.id.infoSnippet);
    tvSnippet.setText(snippet);
  }

  @Override
  public View getInfoWindow(Marker marker)
  {
    addWindowText(marker, custWindow);
    return custWindow;
  }

  @Override
  public View getInfoContents(Marker marker)
  {
    addWindowText(marker, custWindow);
    return custWindow;
  }
}
